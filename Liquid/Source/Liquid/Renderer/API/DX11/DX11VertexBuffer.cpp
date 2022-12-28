#include "LiquidPCH.h"
#include "DX11VertexBuffer.h"

#include "Liquid/Renderer/RenderThread.h"

#include "DX11Device.h"

namespace Liquid {

	DX11VertexBuffer::DX11VertexBuffer(const void* data, uint32 size, VertexBufferUsage usage)
		: m_Usage(usage)
	{
		m_Storage.Allocate(size);
		m_Storage.Insert(data, size);

		Invalidate();
	}

	DX11VertexBuffer::DX11VertexBuffer(uint32 size, VertexBufferUsage usage)
		: m_Usage(usage)
	{
		m_Storage.Allocate(size);
		m_Storage.FillWithZeroes();

		Invalidate();
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{
		RT_SUBMIT_RELEASE(Release)([buffer = m_Buffer]()
		{
			buffer->Release();
		});

		m_Storage.Release();
	}

	void DX11VertexBuffer::Invalidate()
	{
		Ref<DX11VertexBuffer> instance = this;
		RT_SUBMIT(Invalidate)([instance]() mutable
		{
			instance->RT_Invalidate();
		});
	}

	void DX11VertexBuffer::RT_Invalidate()
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = m_Storage.GetSize();
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		if (m_Usage == VertexBufferUsage::Dynamic)
		{
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
		}

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = m_Storage.GetData();
		initData.SysMemPitch = m_Storage.GetSize();
		initData.SysMemSlicePitch = 0;

		DX_CHECK(device->CreateBuffer(&bufferDesc, &initData, &m_Buffer));
	}

	void DX11VertexBuffer::Bind() const
	{
		Ref<const DX11VertexBuffer> instance = this;
		RT_SUBMIT(Invalidate)([instance]()
		{
			instance->RT_Bind();
		});
	}

	void DX11VertexBuffer::RT_Bind() const
	{
		// This will be set later in the pipeline
		s_CurrentlyBoundBuffer = m_Buffer;
	}
	
}