#include "LiquidPCH.h"
#include "DX11Context.h"

#include "DX11Device.h"

namespace Liquid {

	DX11Context::DX11Context(const GraphicsContextCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		GraphicsDeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.EnableDebugLayers = createInfo.EnableDebugLayers;

		m_Device = DX11Device::Select(deviceCreateInfo);
		LQ_ASSERT(m_Device, "Device is nullptr");

		if (createInfo.EnableDebugLayers)
		{
			DXRef<ID3D11Device> device = m_Device->GetDevice();

			DX_CHECK(device->QueryInterface(DX_RIID(ID3D11Debug), &m_DebugLayer));
			m_DebugLayer->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);

			DXRef<ID3D11InfoQueue> infoQueue;
			device->QueryInterface(DX_RIID(ID3D11InfoQueue), &infoQueue);
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