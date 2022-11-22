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
	}

}