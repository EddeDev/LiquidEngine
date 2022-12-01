#include "LiquidPCH.h"
#include "SplashScreen.h"

#include "Windows/WindowsSplashScreen.h"

namespace Liquid {

	Ref<SplashScreen> SplashScreen::Create(const SplashScreenCreateInfo& createInfo)
	{
#ifdef LQ_PLATFORM_WINDOWS
		return Ref<WindowsSplashScreen>::Create(createInfo);
#else
		LQ_VERIFY(false, "Unknown platform");
		return nullptr;
#endif
	}

}