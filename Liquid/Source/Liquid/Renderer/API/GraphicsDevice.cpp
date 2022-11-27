#include "LiquidPCH.h"
#include "GraphicsDevice.h"

#include "Liquid/Core/Application.h"

#include "DX11/DX11Device.h"

namespace Liquid {

	Ref<GraphicsDevice> GraphicsDevice::Select(const GraphicsDeviceCreateInfo& createInfo)
	{
		switch (Application::GetGraphicsAPI())
		{
		case GraphicsAPI::DX11: return DX11Device::Select(createInfo);
		}
		LQ_VERIFY(false, "Unknown Graphics API");
		return nullptr;
	}

}