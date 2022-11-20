#include "LiquidPCH.h"
#include "Swapchain.h"

#include "Liquid/Core/Application.h"

#include "DX11/DX11Swapchain.h"

namespace Liquid {

	Ref<Swapchain> Swapchain::Create(const SwapchainCreateInfo& createInfo)
	{
		switch (Application::GetGraphicsAPI())
		{
		case GraphicsAPI::DX11: return Ref<DX11Swapchain>::Create(createInfo);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

}