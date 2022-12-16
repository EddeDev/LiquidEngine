#include "LiquidPCH.h"
#include "Swapchain.h"

#include "Liquid/Core/Engine.h"

#include "DX11/DX11Swapchain.h"

namespace Liquid {

	Ref<Swapchain> Swapchain::Create(const SwapchainCreateInfo& createInfo)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11Swapchain>::Create(createInfo);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

}