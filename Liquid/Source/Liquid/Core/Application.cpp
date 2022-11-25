#include "LiquidPCH.h"
#include "Application.h"

#include "Window/Window.h"

#include "Liquid/Renderer/ImGuiRenderer.h"

#include <glfw/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace Liquid {

	Ref<Window> Application::s_Window;
	Ref<GraphicsContext> Application::s_Context;
	Ref<Swapchain> Application::s_Swapchain;
	Unique<ThemeCreator> Application::s_ThemeCreator;
	bool Application::s_Running = true;
	bool Application::s_Minimized = false;

	void Application::Init(const ApplicationCreateInfo& createInfo)
	{
		String currentGraphicsAPI = GraphicsAPIUtils::GetGraphicsAPIName(GetGraphicsAPI());
		LQ_INFO_ARGS("Graphics API: {0}", currentGraphicsAPI);

		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Width = 1280;
		windowCreateInfo.Height = 720;
		windowCreateInfo.Title = "Liquid Engine";
		windowCreateInfo.Fullscreen = false;
		windowCreateInfo.Maximize = true;
		// windowCreateInfo.Decorated = false;

		s_Window = Window::Create(windowCreateInfo);
		s_Window->AddCloseCallback(LQ_BIND_CALLBACK(OnWindowCloseCallback));
		s_Window->AddWindowSizeCallback(LQ_BIND_CALLBACK(OnWindowSizeCallback));

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
		swapchainCreateInfo.InitialWidth = s_Window->GetWidth();
		swapchainCreateInfo.InitialHeight = s_Window->GetHeight();
		swapchainCreateInfo.InitialFullscreenState = s_Window->IsFullscreen();
		swapchainCreateInfo.AllowTearing = true;
		swapchainCreateInfo.ColorFormat = PixelFormat::RGBA;
		swapchainCreateInfo.DepthFormat = PixelFormat::DEPTH24_STENCIL8;
		swapchainCreateInfo.BufferCount = 3;
		swapchainCreateInfo.SampleCount = 1;
		s_Swapchain = Swapchain::Create(swapchainCreateInfo);

		s_Window->SetVisible(true);

		ImGuiRenderer::Init();

		s_ThemeCreator = CreateUnique<ThemeCreator>();
	}

	void Application::Shutdown()
	{
		ImGuiRenderer::Shutdown();

		s_Window = nullptr;
	}

	void Application::Run()
	{
		float lastTime = static_cast<float>(glfwGetTime());
		uint32 frames = 0;
		uint32 fps = 0;

		while (s_Running)
		{
			float time = static_cast<float>(glfwGetTime());
			frames++;
			if (time >= lastTime + 1.0f)
			{
				// LQ_INFO_ARGS("{0} fps", frames);
				fps = frames;
				frames = 0;
				lastTime = time;
			}

			s_Window->PollEvents();

			if (!s_Minimized)
			{
				s_Swapchain->BeginFrame();
				s_Swapchain->Clear(BUFFER_COLOR | BUFFER_DEPTH);

				ImGuiRenderer::BeginFrame();

				s_ThemeCreator->Render();

				ImGui::Begin("Liquid Engine");
				ImGui::Text("%d fps", fps);

				bool vsync = s_Swapchain->IsVSyncEnabled();
				if (ImGui::Checkbox("V-Sync", &vsync))
					s_Swapchain->SetVSync(vsync);

				ImGui::End();

				ImGui::ShowDemoWindow();

				ImGuiRenderer::EndFrame();

				s_Swapchain->Present();
			}
		}
	}

	void Application::OnWindowCloseCallback()
	{
		s_Running = false;
	}

	void Application::OnWindowSizeCallback(uint32 width, uint32 height)
	{
		if (width == 0 || height == 0)
		{
			s_Minimized = true;
			return;
		}

		if (s_Minimized)
			s_Minimized = false;

		s_Swapchain->Resize(width, height, s_Window->IsFullscreen());
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
		static_assert(false, "Unknown build configuration");
		return BuildConfiguration::None;
#endif
	}

	GraphicsAPI Application::GetGraphicsAPI()
	{
		return GraphicsAPI::DX11;
	}

}