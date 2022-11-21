#include "LiquidPCH.h"
#include "DX11Device.h"

#include "DX11Context.h"

namespace Liquid {

	namespace Utils {

		static std::vector<DXRef<IDXGIAdapter>> EnumerateAdapters()
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
	}

	DX11Device::~DX11Device()
	{

	}

	Ref<DX11Device> DX11Device::Select(const GraphicsDeviceCreateInfo& createInfo)
	{
		std::vector<DXRef<IDXGIAdapter>> adapters = Utils::EnumerateAdapters();
		uint32 index = 0;
		for (DXRef<IDXGIAdapter> adapter : adapters)
		{
			DXGI_ADAPTER_DESC desc;
			adapter->GetDesc(&desc);

			LQ_TRACE_ARGS("Adapter {0}:", index);
			LQ_TRACE_ARGS("  Description: {0}", StringUtils::FromWideString(desc.Description));
			LQ_TRACE_ARGS("  Vendor: {0}", GraphicsDeviceUtils::VendorToString(static_cast<GraphicsDeviceVendor>(desc.VendorId)));
			LQ_TRACE_ARGS("  Device ID: {0}", desc.DeviceId);
			LQ_TRACE_ARGS("  System ID: {0}", desc.SubSysId);
			LQ_TRACE_ARGS("  Revision: {0}", desc.Revision);
			LQ_TRACE_ARGS("  Dedicated VRAM: {0}", desc.DedicatedVideoMemory);
			LQ_TRACE_ARGS("  Dedicated RAM: {0}", desc.DedicatedVideoMemory);
			LQ_TRACE_ARGS("  Shared RAM: {0}", desc.SharedSystemMemory);
			LQ_TRACE_ARGS("  Adapter LUID: [{0}, {1}]", desc.AdapterLuid.LowPart, desc.AdapterLuid.HighPart);

			index++;
		}

		return Ref<DX11Device>::Create(createInfo, adapters[0]);
	}

}