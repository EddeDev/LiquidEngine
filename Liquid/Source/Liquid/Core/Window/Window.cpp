#include "LiquidPCH.h"
#include "Window.h"

#ifdef LQ_PLATFORM_WINDOWS
#include "Windows/WindowsWindow.h"
#endif

namespace Liquid {

	Ref<Window> Window::Create(const WindowCreateInfo& createInfo)
	{
#ifdef LQ_PLATFORM_WINDOWS
		return Ref<WindowsWindow>::Create(createInfo);
#else
		LQ_VERIFY(false, "Unknown platform");
		return nullptr;
#endif
	}

}