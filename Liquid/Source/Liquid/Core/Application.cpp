#include "LiquidPCH.h"
#include "Application.h"

#include "Window/Window.h"

namespace Liquid {

	Ref<Window> Application::s_Window;
	bool Application::s_Running = true;

	void Application::Init(const ApplicationCreateInfo& createInfo)
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Width = 1280;
		windowCreateInfo.Height = 720;
		windowCreateInfo.Title = "Liquid Engine";

		s_Window = Window::Create(windowCreateInfo);
	}

	void Application::Shutdown()
	{
		s_Window = nullptr;
	}

	void Application::Run()
	{
		while (s_Running)
		{
			s_Window->PollEvents();
			s_Window->SwapBuffers();
		}
	}

	BuildConfiguration Application::GetBuildConfiguration()
	{
#ifdef LQ_BUILD_DEBUG
		return BuildConfiguration::Debug;
#elif LQ_BUILD_RELEASE
		return BuildConfiguration::Release;
#elif LQ_BUILD_SHIPPING
		return BuildConfiguration::Shipping;
#else
		return BuildConfiguration::None;
#endif
	}

	GraphicsAPI Application::GetGraphicsAPI()
	{
		return GraphicsAPI::DX11;
	}

}