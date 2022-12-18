#include "LiquidPCH.h"
#include "DX11Buffer.h"

#include "Liquid/Renderer/RenderThread.h"

#include "DX11Device.h"
#include "DX11StateManager.h"

namespace Liquid {

	DX11Buffer::DX11Buffer(const void* data, uint32 size, BufferUsage usage)
		: m_Usage(usage)
	{
		LQ_CHECK(data);
		LQ_CHECK(size > 0);

		m_Storage.Allocate(size);
		m_Storage.Insert(data, size);

		Ref<DX11Buffer> instance = this;
		RT_SUBMIT(Bind)([instance]() mutable
		{
			DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
			DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

			D3D11_BUFFER_DESC bufferDesc = {};
			bufferDesc.ByteWidth = instance->m_Storage.GetSize();

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_DYNAMIC | BUFFER_USAGE_VOLATILE))
			{
				bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
			}

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_VERTEX_BUFFER))
				bufferDesc.BindFlags |= D3D11_BIND_VERTEX_BUFFER;

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_INDEX_BUFFER))
				bufferDesc.BindFlags |= D3D11_BIND_INDEX_BUFFER;

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_SHADER_RESOURCE))
				bufferDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_UNORDERED_ACCESS))
				bufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_DRAW_INDIRECT))
				bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_SHARED))
				bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_SHARED;

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = instance->m_Storage.GetData();
			initData.SysMemPitch = instance->m_Storage.GetSize();
			initData.SysMemSlicePitch = 0;

			DX_CHECK(device->CreateBuffer(&bufferDesc, &initData, &instance->m_Buffer));
		});
	}

	DX11Buffer::DX11Buffer(uint32 size, BufferUsage usage)
		: m_Usage(usage)
	{
		LQ_CHECK(size > 0);

		m_Storage.Allocate(size);
		m_Storage.FillWithZeroes();

		Ref<DX11Buffer> instance = this;
		RT_SUBMIT(Bind)([instance]() mutable
		{
			DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
			DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

			D3D11_BUFFER_DESC bufferDesc = {};
			bufferDesc.ByteWidth = instance->m_Storage.GetSize();

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_DYNAMIC | BUFFER_USAGE_VOLATILE))
			{
				bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
			}

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_VERTEX_BUFFER))
				bufferDesc.BindFlags |= D3D11_BIND_VERTEX_BUFFER;

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_INDEX_BUFFER))
				bufferDesc.BindFlags |= D3D11_BIND_INDEX_BUFFER;

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_SHADER_RESOURCE))
				bufferDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_UNORDERED_ACCESS))
				bufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_DRAW_INDIRECT))
				bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;

			if (Enum::HasAnyFlags(instance->m_Usage, BUFFER_USAGE_SHARED))
				bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_SHARED;

			DX_CHECK(device->CreateBuffer(&bufferDesc, nullptr, &instance->m_Buffer));
		});
	}

	DX11Buffer::~DX11Buffer()
	{
		RT_SUBMIT_RELEASE(Release)([buffer = m_Buffer]()
		{
			buffer->Release();
		});

		m_Storage.Release();
	}

}