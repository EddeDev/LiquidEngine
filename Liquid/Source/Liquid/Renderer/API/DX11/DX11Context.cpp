#include "LiquidPCH.h"
#include "DX11Context.h"

namespace Liquid {

	DX11Context::DX11Context(const GraphicsContextCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		uint32 flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
		flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		if (createInfo.EnableDebugLayers)
			flags |= D3D11_CREATE_DEVICE_DEBUG;

		DX_CHECK(D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			flags,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&m_Device,
			&m_FeatureLevel,
			&m_DeviceContext
		));

		if (flags & D3D11_CREATE_DEVICE_DEBUG)
		{
			DX_CHECK(m_Device->QueryInterface(DX_RIID(ID3D11Debug), &m_DebugLayer));
			m_DebugLayer->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);

			DXRef<ID3D11InfoQueue> infoQueue;
			m_Device->QueryInterface(DX_RIID(ID3D11InfoQueue), &infoQueue);
			D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET };
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = 1;
			filter.DenyList.pIDList = hide;
			DX_CHECK(infoQueue->AddStorageFilterEntries(&filter));

			DX_CHECK(infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, true));
			DX_CHECK(infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true));
			DX_CHECK(infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true));
		}
	}

}