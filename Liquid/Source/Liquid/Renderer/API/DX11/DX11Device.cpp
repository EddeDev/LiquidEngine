#include "LiquidPCH.h"
#include "DX11Device.h"

#include "DX11Context.h"

namespace Liquid {

	namespace Utils {

		static std::vector<DXRef<IDXGIAdapter>> GetAdapters()
		{
			DXRef<IDXGIFactory> factory;
			if (FAILED(CreateDXGIFactory(DX_RIID(IDXGIFactory), &factory)))
				return {};

			std::vector<DXRef<IDXGIAdapter>> adapters;
			DXRef<IDXGIAdapter> adapter;
			for (uint32 i = 0; factory->EnumAdapters(i, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; i++)
				adapters.push_back(adapter);
			return adapters;
		}

	}

	DX11Device::DX11Device(const GraphicsDeviceCreateInfo& createInfo, DXRef<IDXGIAdapter> adapter)
	{
		s_Instance = this;

		uint32 flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
		flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		if (createInfo.EnableDebugLayers)
			flags |= D3D11_CREATE_DEVICE_DEBUG;

		D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
		if (adapter)
			driverType = D3D_DRIVER_TYPE_UNKNOWN;

		DX_CHECK(D3D11CreateDevice(
			adapter ? adapter.Get() : NULL,
			driverType,
			NULL,
			flags,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&m_Device,
			&m_FeatureLevel,
			&m_DeviceContext
		));

		if (createInfo.EnableDebugLayers)
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

		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);
		m_Info.Vendor = static_cast<GraphicsDeviceVendor>(desc.VendorId);
		// TODO

		m_Info.DedicatedVideoMemory = desc.DedicatedVideoMemory;
		m_Info.DedicatedSystemMemory = desc.DedicatedSystemMemory;
		m_Info.SharedSystemMemory = desc.SharedSystemMemory;
	}

	Ref<DX11Device> DX11Device::Select(const GraphicsDeviceCreateInfo& createInfo)
	{
		std::vector<DXRef<IDXGIAdapter>> adapters = Utils::GetAdapters();

		std::vector<size_t> discreteAdapters;
		size_t intelAdapterIndex = std::numeric_limits<size_t>::max();
		size_t microsoftAdapterIndex = std::numeric_limits<size_t>::max();

		for (size_t i = 0; i < adapters.size(); i++)
		{
			DXRef<IDXGIAdapter> adapter = adapters[i];
			LQ_ASSERT(adapter, "Adapter is nullptr");

			DXGI_ADAPTER_DESC desc;
			adapter->GetDesc(&desc);

			GraphicsDeviceVendor vendor = static_cast<GraphicsDeviceVendor>(desc.VendorId);
			if (vendor == GraphicsDeviceVendor::Intel)
				intelAdapterIndex = i;
			else if (vendor == GraphicsDeviceVendor::Microsoft)
				microsoftAdapterIndex = i;
			else
				discreteAdapters.push_back(i);
		}

		DXRef<IDXGIAdapter> selectedAdapter;
		if (!discreteAdapters.empty())
		{
			selectedAdapter = adapters[discreteAdapters.front()];
		}
		else if (intelAdapterIndex != std::numeric_limits<size_t>::max())
		{
			selectedAdapter = adapters[intelAdapterIndex];
		}
		else if (microsoftAdapterIndex != std::numeric_limits<size_t>::max())
		{
			selectedAdapter = adapters[microsoftAdapterIndex];
			MessageBox(0, L"Microsoft Basic Render Driver was chosen as graphics device", L"Warning", MB_SYSTEMMODAL | MB_ICONWARNING);
		}

		if (!selectedAdapter)
		{
			std::wstring message;
			message += L"Adapters:\n";

			for (size_t i = 0; i < adapters.size(); i++)
			{
				DXRef<IDXGIAdapter> adapter = adapters[i];

				message += L"\n";

				if (!adapter)
				{
					message += StringUtils::ToWideString(fmt::format("Adapter {0}: nullptr\n", i));
				}
				else
				{
					DXGI_ADAPTER_DESC desc;
					adapter->GetDesc(&desc);

					GraphicsDeviceVendor vendor = static_cast<GraphicsDeviceVendor>(desc.VendorId);

					message += StringUtils::ToWideString(fmt::format("Adapter {0}:\n", i));
					message += StringUtils::ToWideString(fmt::format("  Description: {0}\n", StringUtils::FromWideString(desc.Description)));
					message += StringUtils::ToWideString(fmt::format("  Vendor: {0}\n", GraphicsDeviceUtils::VendorToString(vendor)));
					message += StringUtils::ToWideString(fmt::format("  Device ID: {0}\n", desc.DeviceId));
					message += StringUtils::ToWideString(fmt::format("  SubSys ID: {0}\n", desc.SubSysId));
					message += StringUtils::ToWideString(fmt::format("  Revision: {0}\n", desc.Revision));
					message += StringUtils::ToWideString(fmt::format("  Dedicated video memory: {0}\n", desc.DedicatedVideoMemory));
					message += StringUtils::ToWideString(fmt::format("  Dedicated system memory: {0}\n", desc.DedicatedSystemMemory));
					message += StringUtils::ToWideString(fmt::format("  Shared system memory: {0}\n", desc.SharedSystemMemory));
					message += StringUtils::ToWideString(fmt::format("  Adapter LUID: [{0}, {1}]\n", desc.AdapterLuid.LowPart, desc.AdapterLuid.HighPart));
				}
			}
			
			if (IsDebuggerPresent())
			{
				message += L"\n";
				message += L"Do you want to debug the crash?";
			}

			uint32 messageType = MB_SYSTEMMODAL | MB_ICONERROR;
			if (IsDebuggerPresent())
				messageType |= MB_YESNO;

			int32 result = MessageBox(0, message.c_str(), L"Failed to choose a DX11 Adapter.", messageType);
			if (IsDebuggerPresent())
			{
				if (result == IDYES)
					DebugBreak();
			}

			exit(EXIT_FAILURE);
			return nullptr;
		}

		DXGI_ADAPTER_DESC desc;
		selectedAdapter->GetDesc(&desc);
		LQ_TRACE_ARGS("Chosen DX11 Adapter: {0}", StringUtils::FromWideString(desc.Description));

		return Ref<DX11Device>::Create(createInfo, selectedAdapter);
	}

}