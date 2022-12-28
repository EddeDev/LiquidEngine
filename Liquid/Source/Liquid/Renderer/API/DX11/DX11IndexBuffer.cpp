#include "LiquidPCH.h"
#include "DX11IndexBuffer.h"

#include "Liquid/Renderer/RenderThread.h"

#include "DX11Device.h"

namespace Liquid {

	namespace Utils {

		static DXGI_FORMAT IndexBufferDataTypeToDX11Format(IndexBufferDataType type)
		{
			switch (type)
			{
			case IndexBufferDataType::UnsignedInt:   return DXGI_FORMAT_R32_UINT;
			case IndexBufferDataType::UnsignedShort: return DXGI_FORMAT_R16_UINT;
			case IndexBufferDataType::UnsignedByte:  return DXGI_FORMAT_R8_UINT;
			}
			LQ_VERIFY(false, "Unknown IndexBufferDataType");
			return DXGI_FORMAT_UNKNOWN;
		}

	}

	DX11IndexBuffer::DX11IndexBuffer(const void* data, uint32 size, IndexBufferDataType dataType)
		: m_DataType(dataType)
	{
		m_Storage.Allocate(size);
		m_Storage.Insert(data, size);

		Invalidate();
	}

	DX11IndexBuffer::DX11IndexBuffer(uint32 size, IndexBufferDataType dataType)
		: m_DataType(dataType)
	{
		m_Storage.Allocate(size);
		m_Storage.FillWithZeroes();

		Invalidate();
	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{
		RT_SUBMIT_RELEASE(Release)([buffer = m_Buffer]()
		{
			buffer->Release();
		});

		m_Storage.Release();
	}

	void DX11IndexBuffer::Invalidate()
	{
		Ref<DX11IndexBuffer> instance = this;
		RT_SUBMIT(Invalidate)([instance]() mutable
		{
			instance->RT_Invalidate();
		});
	}

	void DX11IndexBuffer::RT_Invalidate()
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = m_Storage.GetSize();
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = m_Storage.GetData();
		initData.SysMemPitch = m_Storage.GetSize();
		initData.SysMemSlicePitch = 0;

		DX_CHECK(device->CreateBuffer(&bufferDesc, &initData, &m_Buffer));
	}

	void DX11IndexBuffer::Bind() const
	{
		Ref<const DX11IndexBuffer> instance = this;
		RT_SUBMIT(Invalidate)([instance]()
		{
			instance->RT_Bind();
		});
	}

	void DX11IndexBuffer::RT_Bind() const
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		DXGI_FORMAT format = Utils::IndexBufferDataTypeToDX11Format(m_DataType);
		deviceContext->IASetIndexBuffer(m_Buffer, format, 0);
	}
	
}