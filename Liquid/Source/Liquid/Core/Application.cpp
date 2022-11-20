#include "LiquidPCH.h"
#include "Application.h"

#include "Window/Window.h"

#include <glfw/glfw3.h>

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
		float lastTime = glfwGetTime();
		uint32 frames = 0;

		while (s_Running)
		{
			float time = glfwGetTime();
			frames++;
			if (time >= lastTime + 1.0f)
			{
				LQ_INFO_ARGS("{0} fps", frames);
				frames = 0;
				lastTime = time;
			}

			s_Window->PollEvents();
			if (s_Window->IsCloseRequested())
				s_Running = false;

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