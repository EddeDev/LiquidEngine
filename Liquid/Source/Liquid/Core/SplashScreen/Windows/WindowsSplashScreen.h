#pragma once

#ifdef LQ_PLATFORM_WINDOWS

#include "Liquid/Core/SplashScreen/SplashScreen.h"

namespace Liquid {

	class WindowsSplashScreen : public SplashScreen
	{
	public:
		WindowsSplashScreen(const SplashScreenCreateInfo& createInfo);
		virtual ~WindowsSplashScreen();
	private:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		SplashScreenCreateInfo m_CreateInfo;

		HWND m_HWND = nullptr;
		ATOM m_WindowClass = NULL;
	};

}

#endif