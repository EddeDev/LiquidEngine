#include "LiquidPCH.h"
#include "DX11Pipeline.h"

#include "Liquid/Renderer/RenderThread.h"

#include "DX11Device.h"
#include "DX11Shader.h"
#include "DX11Buffer.h"

namespace Liquid {
	
	namespace Utils {

		static D3D_PRIMITIVE_TOPOLOGY DX11PrimitiveTopology(PrimitiveTopology topology)
		{
			switch (topology)
			{
			case PrimitiveTopology::Triangles: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			}
			LQ_VERIFY(false, "Unknown PrimitiveTopology");
			return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}

		static DXGI_FORMAT ShaderDataTypeToDX11Type(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float: return DXGI_FORMAT_R32_FLOAT;
			case ShaderDataType::Vec2:  return DXGI_FORMAT_R32G32_FLOAT;
			case ShaderDataType::Vec3:  return DXGI_FORMAT_R32G32B32_FLOAT;
			case ShaderDataType::Vec4:  return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case ShaderDataType::Int:   return DXGI_FORMAT_R32_SINT;
			case ShaderDataType::IVec2: return DXGI_FORMAT_R32G32_SINT;
			case ShaderDataType::IVec3: return DXGI_FORMAT_R32G32B32_SINT;
			case ShaderDataType::IVec4: return DXGI_FORMAT_R32G32B32A32_SINT;
			}
			LQ_VERIFY(false, "Unknown ShaderDataType");
			return DXGI_FORMAT_UNKNOWN;
		}

	}

	DX11Pipeline::DX11Pipeline(const GraphicsPipelineCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		Invalidate();
	}

	DX11Pipeline::~DX11Pipeline()
	{
		RT_SUBMIT_RELEASE(Release)([inputLayout = m_InputLayout, rasterizerState = m_RasterizerState, depthStencilState = m_DepthStencilState]()
		{
			if (inputLayout)
				inputLayout->Release();
			if (rasterizerState)
				rasterizerState->Release();
			if (depthStencilState)
				depthStencilState->Release();
		});
	}

	void DX11Pipeline::Invalidate()
	{
		Ref<DX11Pipeline> instance = this;
		RT_SUBMIT(Invalidate)([instance]() mutable
		{
			instance->RT_Invalidate();
		});
	}

	void DX11Pipeline::RT_Invalidate()
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		if (m_InputLayout)
			m_InputLayout->Release();
		if (m_RasterizerState)
			m_RasterizerState->Release();
		if (m_DepthStencilState)
			m_DepthStencilState->Release();

		LQ_CHECK(m_CreateInfo.InputLayout.GetAttributeCount() > 0);

		std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
		uint32 attributeIndex = 0;
		for (const auto& attribute : m_CreateInfo.InputLayout)
		{
			D3D11_INPUT_ELEMENT_DESC elementDesc = {};
			elementDesc.SemanticName = attribute.Name.c_str();
			elementDesc.SemanticIndex = attributeIndex;
			elementDesc.Format = Utils::ShaderDataTypeToDX11Type(attribute.Type);
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = attribute.Offset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			elements.push_back(elementDesc);
			attributeIndex++;
		}

		Ref<DX11Shader> shader = m_CreateInfo.Shader.As<DX11Shader>();
		LQ_CHECK(shader);

		auto& shaderData = shader->GetShaderData();
		DX_CHECK(device->CreateInputLayout(elements.data(), m_CreateInfo.InputLayout.GetAttributeCount(), shaderData.VertexShaderBlob->GetBufferPointer(), shaderData.VertexShaderBlob->GetBufferSize(), &m_InputLayout));

		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = m_CreateInfo.Wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = m_CreateInfo.BackfaceCulling ? D3D11_CULL_BACK : D3D11_CULL_NONE;
		rasterizerDesc.FrontCounterClockwise = true;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.AntialiasedLineEnable = true;
		DX_CHECK(device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState));

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = m_CreateInfo.DepthTest;
		depthStencilDesc.DepthWriteMask = m_CreateInfo.DepthWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = 0xff;
		depthStencilDesc.StencilWriteMask = 0xff;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		DX_CHECK(device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState));
	}

	void DX11Pipeline::DrawIndexed(Ref<Buffer> vertexBuffer, Ref<Buffer> indexBuffer) const
	{
		Ref<const DX11Pipeline> instance = this;
		RT_SUBMIT(DrawIndexed)([instance, vertexBuffer, indexBuffer]()
		{
			DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
			DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

			instance->m_CreateInfo.Shader->RT_Bind();

			deviceContext->IASetInputLayout(instance->m_InputLayout);
			deviceContext->RSSetState(instance->m_RasterizerState);
			deviceContext->OMSetDepthStencilState(instance->m_DepthStencilState, 0);

			if (vertexBuffer)
			{
				uint32 offset = instance->m_CreateInfo.InputLayout.GetOffset();
				uint32 stride = instance->m_CreateInfo.InputLayout.GetStride();
				ID3D11Buffer* buffer = vertexBuffer.As<DX11Buffer>()->GetBuffer();
				deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
			}

			if (indexBuffer)
			{
				DXGI_FORMAT format = DXGI_FORMAT_R32_UINT;
				deviceContext->IASetIndexBuffer(indexBuffer.As<DX11Buffer>()->GetBuffer(), format, 0);
			}
			
			deviceContext->IASetPrimitiveTopology(Utils::DX11PrimitiveTopology(instance->m_CreateInfo.Topology));
			deviceContext->DrawIndexed(indexBuffer->GetSize() / sizeof(uint32), 0, 0);

			instance->m_CreateInfo.Shader->RT_Unbind();
		});
	}

}