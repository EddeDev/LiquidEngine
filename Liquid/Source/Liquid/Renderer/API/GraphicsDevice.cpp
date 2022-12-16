#include "LiquidPCH.h"
#include "GraphicsDevice.h"

#include "Liquid/Core/Engine.h"

#include "DX11/DX11Device.h"

namespace Liquid {

	Ref<GraphicsDevice> GraphicsDevice::Select(const GraphicsDeviceCreateInfo& createInfo)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return DX11Device::Select(createInfo);
		}
		LQ_VERIFY(false, "Unknown Graphics API");
		return nullptr;
	}

}