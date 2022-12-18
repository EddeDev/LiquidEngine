#include "LiquidPCH.h"
#include "DX11Shader.h"

#include "Liquid/Renderer/RenderThread.h"

#include "DX11Device.h"
#include "DX11StateManager.h"

namespace Liquid {

	namespace Utils {

		static String ReadFile(const std::string& filepath)
		{
			String result;
			std::ifstream in(filepath, std::ios::in | std::ios::binary);
			if (in)
			{
				in.seekg(0, std::ios::end);
				std::streampos size = in.tellg();
				in.seekg(0, std::ios::beg);
				result.resize(size);
				in.read(result.data(), size);
			}
			else
			{
				LQ_VERIFY(false, "Could not open file '{0}'", filepath);
			}
			in.close();
			return result;
		}

		static String GetShaderProfile(ShaderStage stage, const String& version)
		{
			switch (stage)
			{
			case ShaderStage::Vertex:   return fmt::format("vs_{0}", version);
			case ShaderStage::Pixel: return fmt::format("ps_{0}", version);
			case ShaderStage::Compute:  return fmt::format("cs_{0}", version);
			}
			LQ_VERIFY(false, "Unknown shader stage");
			return "";
		}

	}

	DX11Shader::DX11Shader(const String& vertexShaderPath, const String& fragmentShaderPath)
	{
		std::unordered_map<ShaderStage, String> sources;
		sources[ShaderStage::Vertex] = Utils::ReadFile(vertexShaderPath);
		sources[ShaderStage::Pixel] = Utils::ReadFile(fragmentShaderPath);

		Ref<DX11Shader> instance = this;
		RT_SUBMIT(Compile)([instance, sources]() mutable
		{
			instance->Compile(sources);
		});
	}

	DX11Shader::~DX11Shader()
	{
		RT_SUBMIT(Compile)([shaderData = m_ShaderData]() mutable
		{
			if (shaderData.VertexShaderBlob)   shaderData.VertexShaderBlob->Release();
			if (shaderData.PixelShaderBlob)    shaderData.PixelShaderBlob->Release();
			if (shaderData.ComputeShaderBlob)  shaderData.ComputeShaderBlob->Release();

			if (shaderData.VertexShader)       shaderData.VertexShader->Release();
			if (shaderData.PixelShader)        shaderData.PixelShader->Release();
			if (shaderData.ComputeShader)      shaderData.ComputeShader->Release();
		});
	}

	void DX11Shader::Bind() const
	{
		Ref<const DX11Shader> instance = this;
		RT_SUBMIT(Compile)([instance]()
		{
			if (instance->m_ShaderData.VertexShader)
				DX11StateManager::BindShader(ShaderStage::Vertex, instance->m_ShaderData.VertexShader);
			if (instance->m_ShaderData.PixelShader)
				DX11StateManager::BindShader(ShaderStage::Pixel, instance->m_ShaderData.PixelShader);
			if (instance->m_ShaderData.ComputeShader)
				DX11StateManager::BindShader(ShaderStage::Compute, instance->m_ShaderData.ComputeShader);
		});
	}

	void DX11Shader::Unbind() const
	{
		Ref<const DX11Shader> instance = this;
		RT_SUBMIT(Compile)([instance]()
		{
			if (instance->m_ShaderData.VertexShader)
				DX11StateManager::UnbindShader(ShaderStage::Vertex);
			if (instance->m_ShaderData.PixelShader)
				DX11StateManager::UnbindShader(ShaderStage::Pixel);
			if (instance->m_ShaderData.ComputeShader)
				DX11StateManager::UnbindShader(ShaderStage::Compute);
		});
	}

	void DX11Shader::RT_Bind() const
	{
		if (m_ShaderData.VertexShader)
			DX11StateManager::BindShader(ShaderStage::Vertex, m_ShaderData.VertexShader);
		if (m_ShaderData.PixelShader)
			DX11StateManager::BindShader(ShaderStage::Pixel, m_ShaderData.PixelShader);
		if (m_ShaderData.ComputeShader)
			DX11StateManager::BindShader(ShaderStage::Compute, m_ShaderData.ComputeShader);
	}

	void DX11Shader::RT_Unbind() const
	{
		if (m_ShaderData.VertexShader)
			DX11StateManager::UnbindShader(ShaderStage::Vertex);
		if (m_ShaderData.PixelShader)
			DX11StateManager::UnbindShader(ShaderStage::Pixel);
		if (m_ShaderData.ComputeShader)
			DX11StateManager::UnbindShader(ShaderStage::Compute);
	}

	void DX11Shader::Compile(const std::unordered_map<ShaderStage, String>& sources)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();

		if (m_ShaderData.VertexShaderBlob)   m_ShaderData.VertexShaderBlob->Release();
		if (m_ShaderData.PixelShaderBlob)    m_ShaderData.PixelShaderBlob->Release();
		if (m_ShaderData.ComputeShaderBlob)  m_ShaderData.ComputeShaderBlob->Release();

		if (m_ShaderData.VertexShader)       m_ShaderData.VertexShader->Release();
		if (m_ShaderData.PixelShader)        m_ShaderData.PixelShader->Release();
		if (m_ShaderData.ComputeShader)      m_ShaderData.ComputeShader->Release();

		uint32 compileFlags = 0;
#ifdef LQ_BUILD_DEBUG
		compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_PREFER_FLOW_CONTROL;
#else
		compileFlags |= D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

		for (auto&& [stage, source] : sources)
		{
			ID3DBlob* shaderBlob = nullptr;
			ID3DBlob* errorBlob = nullptr;
			HRESULT hr = D3DCompile(
				source.c_str(),
				source.size(),
				"",
				nullptr,
				nullptr,
				"main",
				Utils::GetShaderProfile(stage, "5_0").c_str(),
				compileFlags,
				0,
				&shaderBlob,
				&errorBlob
			);

			if (FAILED(hr))
			{
				if (errorBlob)
				{
					LQ_ERROR_ARGS(static_cast<char*>(errorBlob->GetBufferPointer()));
					errorBlob->Release();
				}

				if (shaderBlob)
					shaderBlob->Release();
			}
			else
			{
				switch (stage)
				{
				case ShaderStage::Vertex:
				{
					m_ShaderData.VertexShaderBlob = shaderBlob;
					DX_CHECK(device->CreateVertexShader(m_ShaderData.VertexShaderBlob->GetBufferPointer(), m_ShaderData.VertexShaderBlob->GetBufferSize(), nullptr, &m_ShaderData.VertexShader));
					break;
				}
				case ShaderStage::Pixel:
				{
					m_ShaderData.PixelShaderBlob = shaderBlob;
					DX_CHECK(device->CreatePixelShader(m_ShaderData.PixelShaderBlob->GetBufferPointer(), m_ShaderData.PixelShaderBlob->GetBufferSize(), nullptr, &m_ShaderData.PixelShader));
					break;
				}
				case ShaderStage::Compute:
				{
					m_ShaderData.ComputeShaderBlob = shaderBlob;
					DX_CHECK(device->CreateComputeShader(m_ShaderData.ComputeShaderBlob->GetBufferPointer(), m_ShaderData.ComputeShaderBlob->GetBufferSize(), nullptr, &m_ShaderData.ComputeShader));
					break;
				}
				}
			}
		}
	}

}