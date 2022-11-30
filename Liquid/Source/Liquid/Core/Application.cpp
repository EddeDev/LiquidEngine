#include "LiquidPCH.h"
#include "Application.h"

#include "Window/Window.h"

#include "Liquid/Renderer/ImGuiRenderer.h"

#include <glfw/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace Liquid {

	Ref<Window> Application::s_MainWindow;
	Ref<GraphicsDevice> Application::s_Device;
	Ref<GraphicsContext> Application::s_Context;
	Ref<Swapchain> Application::s_Swapchain;
	Ref<ImGuiRenderer> Application::s_ImGuiRenderer;
	Unique<ThemeBuilder> Application::s_ThemeBuilder;
	bool Application::s_Running = true;
	bool Application::s_Minimized = false;

	void Application::Init(const ApplicationCreateInfo& createInfo)
	{
		String currentGraphicsAPI = GraphicsAPIUtils::GetGraphicsAPIName(GetGraphicsAPI());
		LQ_INFO_ARGS("Graphics API: {0}", currentGraphicsAPI);

		// Device
		{
			GraphicsDeviceCreateInfo deviceCreateInfo;
#ifdef LQ_BUILD_DEBUG
			deviceCreateInfo.EnableDebugLayers = true;
#else
			deviceCreateInfo.EnableDebugLayers = false;
#endif
			s_Device = GraphicsDevice::Select(deviceCreateInfo);
		}

		// Window
		{
			WindowCreateInfo windowCreateInfo;
			windowCreateInfo.Width = 1280;
			windowCreateInfo.Height = 720;
			windowCreateInfo.Title = "Liquid Engine";
			windowCreateInfo.Fullscreen = false;
			windowCreateInfo.Maximize = true;

			s_MainWindow = Window::Create(windowCreateInfo);
			s_MainWindow->AddCloseCallback(LQ_BIND_CALLBACK(OnWindowCloseCallback));
			s_MainWindow->AddWindowSizeCallback(LQ_BIND_CALLBACK(OnWindowSizeCallback));
		}

		// Context
		{
			GraphicsContextCreateInfo contextCreateInfo;
			contextCreateInfo.WindowHandle = s_MainWindow->GetPlatformHandle();
#ifdef LQ_BUILD_DEBUG
			contextCreateInfo.EnableDebugLayers = true;
#else
			contextCreateInfo.EnableDebugLayers = false;
#endif
			s_Context = GraphicsContext::Create(contextCreateInfo);
		}

		// Swapchain
		{
			SwapchainCreateInfo swapchainCreateInfo;
			swapchainCreateInfo.WindowHandle = s_MainWindow->GetPlatformHandle();
			swapchainCreateInfo.InitialWidth = s_MainWindow->GetWidth();
			swapchainCreateInfo.InitialHeight = s_MainWindow->GetHeight();
			swapchainCreateInfo.InitialFullscreenState = s_MainWindow->IsFullscreen();
			swapchainCreateInfo.AllowTearing = true;
			swapchainCreateInfo.ColorFormat = PixelFormat::RGBA;
			swapchainCreateInfo.DepthFormat = PixelFormat::DEPTH24_STENCIL8;
			swapchainCreateInfo.BufferCount = 3;
			swapchainCreateInfo.SampleCount = 1;

			s_Swapchain = Swapchain::Create(swapchainCreateInfo);
		}

		// ImGui
		{
			ImGuiRendererCreateInfo createInfo;
			createInfo.Window = s_MainWindow;
			createInfo.DebugName = "ImGuiRenderer-Main";

			s_ImGuiRenderer = Ref<ImGuiRenderer>::Create(createInfo);
		}

		s_ThemeBuilder = CreateUnique<ThemeBuilder>();

		s_MainWindow->SetVisible(true);
	}

	void Application::Shutdown()
	{
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

			s_MainWindow->PollEvents();

			if (!s_Minimized)
			{
				s_Swapchain->BeginFrame();
				s_Swapchain->Clear(BUFFER_COLOR | BUFFER_DEPTH);

				s_ImGuiRenderer->BeginFrame();

				s_ThemeBuilder->Render();

				ImGui::Begin("Liquid Engine");
				ImGui::Text("%d fps", fps);

				BuildConfiguration buildConfig = GetBuildConfiguration();
				String buildConfigName;
				switch (buildConfig)
				{
				case BuildConfiguration::Debug:    buildConfigName = "Debug"; break;
				case BuildConfiguration::Release:  buildConfigName = "Release"; break;
				case BuildConfiguration::Shipping: buildConfigName = "Shipping"; break;
				}
				ImGui::Text("Build Configuration: %s", buildConfigName.c_str());

				if (ImGui::CollapsingHeader("Graphics Device"))
				{
					auto& deviceInfo = s_Device->GetInfo();
					ImGui::Text("Vendor: %s", GraphicsDeviceUtils::VendorToString(deviceInfo.Vendor));
					// ImGui::Text("Renderer: %s", deviceInfo.Renderer.c_str());
					// String currentGraphicsAPI = GraphicsAPIUtils::GetGraphicsAPIName(GetGraphicsAPI());
					// ImGui::Text("%s Version: %s", currentGraphicsAPI.c_str(), deviceInfo.PlatformVersion.c_str());
				}

				bool vsync = s_Swapchain->IsVSyncEnabled();
				if (ImGui::Checkbox("V-Sync", &vsync))
					s_Swapchain->SetVSync(vsync);

				ImGui::End();

				s_ImGuiRenderer->EndFrame();

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

		s_Swapchain->Resize(width, height, s_MainWindow->IsFullscreen());
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