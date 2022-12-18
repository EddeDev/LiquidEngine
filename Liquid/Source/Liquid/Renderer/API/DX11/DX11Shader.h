#pragma once

#include "Liquid/Renderer/API/Shader.h"

#include "DX11Common.h"

namespace Liquid {

	struct DX11ShaderData
	{
		ID3DBlob* VertexShaderBlob = nullptr;
		ID3DBlob* PixelShaderBlob = nullptr;
		ID3DBlob* ComputeShaderBlob = nullptr;

		ID3D11VertexShader* VertexShader = nullptr;
		ID3D11PixelShader* PixelShader = nullptr;
		ID3D11ComputeShader* ComputeShader = nullptr;
	};

	class DX11Shader : public Shader
	{
	public:
		DX11Shader(const String& vertexShaderPath, const String& fragmentShaderPath);
		virtual ~DX11Shader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void RT_Bind() const override;
		virtual void RT_Unbind() const override;

		const DX11ShaderData& GetShaderData() const { return m_ShaderData; }
	private:
		void Compile(const std::unordered_map<ShaderStage, String>& sources);
	private:
		DX11ShaderData m_ShaderData = {};
	};

}