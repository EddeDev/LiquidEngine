#include "LiquidPCH.h"
#include "DX11Pipeline.h"

#include "Liquid/Renderer/RenderThread.h"

#include "DX11Device.h"
#include "DX11Buffer.h"

namespace Liquid {

	DX11Pipeline::DX11Pipeline(const GraphicsPipelineCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		
	}

	DX11Pipeline::~DX11Pipeline()
	{

	}

	void DX11Pipeline::DrawIndexed(Ref<Buffer> vertexBuffer, Ref<Buffer> indexBuffer) const
	{
		Ref<const DX11Pipeline> instance = this;
		RT_SUBMIT(DrawIndexed)([instance, vertexBuffer, indexBuffer]()
		{
#if 0
			DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
			DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

			// Bind vertex buffer
			uint32 offset = 0;
			uint32 stride = sizeof(float) * 3;
			ID3D11Buffer* buffer = vertexBuffer.As<DX11Buffer>()->GetBuffer();
			deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

			// Bind index buffer
			deviceContext->IASetIndexBuffer(indexBuffer.As<DX11Buffer>()->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
			
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			deviceContext->DrawIndexed(indexBuffer->GetSize() / sizeof(uint32), 0, 0);
#endif
		});
	}

}