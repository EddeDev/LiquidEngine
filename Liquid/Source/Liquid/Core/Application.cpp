#include "LiquidPCH.h"
#include "Application.h"

#include "Window/Window.h"

#include <glfw/glfw3.h>

namespace Liquid {

	Ref<Window> Application::s_Window;
	Ref<GraphicsContext> Application::s_Context;
	Ref<Swapchain> Application::s_Swapchain;
	bool Application::s_Running = true;

	void Application::Init(const ApplicationCreateInfo& createInfo)
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Width = 1280;
		windowCreateInfo.Height = 720;
		windowCreateInfo.Title = "Liquid Engine";

		s_Window = Window::Create(windowCreateInfo);

		GraphicsContextCreateInfo contextCreateInfo;
		contextCreateInfo.WindowHandle = s_Window->GetPlatformHandle();
#ifdef LQ_BUILD_DEBUG
		contextCreateInfo.EnableDebugLayers = true;
#else
		contextCreateInfo.EnableDebugLayers = false;
#endif
		s_Context = GraphicsContext::Create(contextCreateInfo);

		SwapchainCreateInfo swapchainCreateInfo;
		swapchainCreateInfo.WindowHandle = s_Window->GetPlatformHandle();
		swapchainCreateInfo.InitialWidth = s_Window->GetFramebufferWidth();
		swapchainCreateInfo.InitialHeight = s_Window->GetFramebufferHeight();
		swapchainCreateInfo.ColorFormat = PixelFormat::RGBA;
		swapchainCreateInfo.DepthFormat = PixelFormat::DEPTH24_STENCIL8;
		swapchainCreateInfo.BufferCount = 3;
		swapchainCreateInfo.SampleCount = 1;
		s_Swapchain = Swapchain::Create(swapchainCreateInfo);

		s_Window->SetVisible(true);
	}

	void Application::Shutdown()
	{
		s_Window = nullptr;
	}

	void Application::Run()
	{
		float lastTime = static_cast<float>(glfwGetTime());
		uint32 frames = 0;

		while (s_Running)
		{
			float time = static_cast<float>(glfwGetTime());
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

			s_Swapchain->Clear(BUFFER_COLOR | BUFFER_DEPTH);
			s_Swapchain->Present();
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