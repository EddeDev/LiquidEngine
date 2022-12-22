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

		static DXGI_FORMAT VertexElementTypeToDX11Format(VertexElementType type)
		{
			switch (type)
			{
			case VertexElementType::Float:  return DXGI_FORMAT_R32_FLOAT;
			case VertexElementType::Float2: return DXGI_FORMAT_R32G32_FLOAT;
			case VertexElementType::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
			case VertexElementType::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case VertexElementType::Int:    return DXGI_FORMAT_R32_SINT;
			case VertexElementType::Int2:   return DXGI_FORMAT_R32G32_SINT;
			case VertexElementType::Int3:   return DXGI_FORMAT_R32G32B32_SINT;
			case VertexElementType::Int4:   return DXGI_FORMAT_R32G32B32A32_SINT;
			}
			LQ_VERIFY(false, "Unknown VertexElementType");
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

		Ref<DX11Shader> shader = m_CreateInfo.Shader.As<DX11Shader>();
		LQ_CHECK(shader);

		auto& shaderData = shader->GetShaderData();

		if (m_CreateInfo.VertexElements.GetCount() > 0 && shaderData.VertexShaderBlob)
		{
			m_VertexElements.clear();
			uint32 elementIndex = 0;
			for (auto& element : m_CreateInfo.VertexElements)
			{
				D3D11_INPUT_ELEMENT_DESC& elementDesc = m_VertexElements.emplace_back();
				elementDesc.SemanticName = element.Name.c_str();
				elementDesc.SemanticIndex = elementIndex;
				elementDesc.Format = Utils::VertexElementTypeToDX11Format(element.Type);
				elementDesc.InputSlot = 0;
				elementDesc.AlignedByteOffset = element.Offset;
				elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				elementDesc.InstanceDataStepRate = 0;
				elementIndex++;
			}

			DX_CHECK(device->CreateInputLayout(m_VertexElements.data(), static_cast<uint32>(m_VertexElements.size()), shaderData.VertexShaderBlob->GetBufferPointer(), shaderData.VertexShaderBlob->GetBufferSize(), &m_InputLayout));
		}

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
				uint32 stride = instance->m_CreateInfo.VertexElements.GetStride();
				uint32 offset = instance->m_CreateInfo.VertexElements.GetOffset();
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