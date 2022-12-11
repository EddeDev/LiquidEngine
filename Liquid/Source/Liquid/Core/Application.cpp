#include "LiquidPCH.h"
#include "Application.h"

#include "Liquid/Renderer/RenderThread.h"
#include "Liquid/Renderer/ImGuiRenderer.h"

#include <glfw/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>

#include "Debugging/Timer.h"
#include "Debugging/ScopedTimer.h"

namespace Liquid {

	Ref<Window> Application::s_MainWindow;
	Ref<GraphicsDevice> Application::s_Device;
	Ref<GraphicsContext> Application::s_Context;
	Ref<Swapchain> Application::s_Swapchain;
	Ref<ImGuiRenderer> Application::s_ImGuiRenderer;
	Unique<ThemeBuilder> Application::s_ThemeBuilder;

	std::queue<std::function<void()>> Application::s_MainThreadQueue;
	std::queue<std::function<void()>> Application::s_UpdateThreadQueue;

	std::mutex Application::s_MainThreadMutex;
	std::mutex Application::s_UpdateThreadMutex;

	std::thread::id Application::s_MainThreadID;
	Unique<Thread> Application::s_UpdateThread;
	Unique<Thread> Application::s_RenderThread;

	std::atomic<bool> Application::s_Running = true;
	std::atomic<bool> Application::s_Minimized = false;

	uint32 Application::s_FPS = 0;

	float Application::s_UpdateThreadTime = 0.0f;
	float Application::s_UpdateThreadWaitTime = 0.0f;
	float Application::s_RenderThreadTime = 0.0f;

	void Application::Init(const ApplicationCreateInfo& createInfo)
	{
		s_MainThreadID = std::this_thread::get_id();

		void* mainThreadHandle = ThreadUtils::CurrentThreadHandle();
		ThreadUtils::SetName(mainThreadHandle, "Main Thread");
		ThreadUtils::SetPriority(mainThreadHandle, ThreadPriority::BelowNormal);

		if (createInfo.ShowSplashScreen)
			SplashScreen::Show();

		RenderThreadQueue::Init();

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

			SplashScreen::SetProgress(10, "Creating device...");
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

			SplashScreen::SetProgress(20, "Creating window...");
			s_MainWindow = Window::Create(windowCreateInfo);
			s_MainWindow->AddCloseCallback(LQ_BIND_CALLBACK(OnWindowCloseCallback));
			s_MainWindow->AddWindowSizeCallback(LQ_BIND_CALLBACK(OnWindowSizeCallback));
		}

		// ImGui
		if (createInfo.EnableImGui)
		{
			ImGuiRendererCreateInfo createInfo;
			createInfo.Window = s_MainWindow;
			createInfo.DebugName = "ImGuiRenderer-Main";
			createInfo.ViewportsEnable = false;

			SplashScreen::SetProgress(30, "Creating ImGui context...");
			s_ImGuiRenderer = Ref<ImGuiRenderer>::Create(createInfo);
		}

		SplashScreen::SetProgress(30, "Creating theme builder...");
		s_ThemeBuilder = CreateUnique<ThemeBuilder>();
	}

	void Application::Shutdown()
	{
		RenderThreadQueue::Shutdown();
	}

	void Application::Run()
	{
		// Render Thread
		ThreadCreateInfo renderThreadCreateInfo;
		renderThreadCreateInfo.Name = "Render Thread";
		renderThreadCreateInfo.Priority = ThreadPriority::Highest;
		s_RenderThread = CreateUnique<Thread>(renderThreadCreateInfo);
		s_RenderThread->PushJob("Render Thread - Create Context", RT_CreateContext);

		// Update Thread
		ThreadCreateInfo updateThreadCreateInfo;
		updateThreadCreateInfo.Name = "Update Thread";
		updateThreadCreateInfo.Priority = ThreadPriority::AboveNormal;
		s_UpdateThread = CreateUnique<Thread>(updateThreadCreateInfo);
		s_UpdateThread->PushJob("Update Thread - Main Loop", UT_MainLoop);

		while (s_Running)
		{
			s_MainWindow->WaitEvents();

			// Execute queue
			{
				std::unique_lock<std::mutex> lock(s_MainThreadMutex);
				while (!s_MainThreadQueue.empty())
				{
					auto& callback = s_MainThreadQueue.front();
					callback();
					s_MainThreadQueue.pop();
				}
			}
		}

		s_UpdateThread->Wait();

		s_UpdateThread.reset();
		s_RenderThread.reset();
	}

#ifdef LQ_BUILD_DEBUG
#define THREAD_CHECKS 1
#else
#define THREAD_CHECKS 0
#endif

	void Application::SubmitToMainThread(std::function<void()> function)
	{
#if THREAD_CHECKS
		if (std::this_thread::get_id() == s_MainThreadID)
			LQ_WARNING_ARGS("Application::SubmitToMainThread was called from the main thread");
		if (std::this_thread::get_id() == s_RenderThread->GetThreadID())
			LQ_WARNING_ARGS("Application::SubmitToMainThread was called from the render thread");
#endif

		std::lock_guard<std::mutex> lock(s_MainThreadMutex);
		s_MainThreadQueue.push(std::move(function));
		s_MainWindow->PostEmptyEvent();
	}

	void Application::SubmitToUpdateThread(std::function<void()> function)
	{
#if THREAD_CHECKS
		if (std::this_thread::get_id() == s_UpdateThread->GetThreadID())
			LQ_WARNING_ARGS("Application::SubmitToUpdateThread was called from the update thread");
		if (std::this_thread::get_id() == s_RenderThread->GetThreadID())
			LQ_WARNING_ARGS("Application::SubmitToUpdateThread was called from the render thread");
#endif

		std::lock_guard<std::mutex> lock(s_UpdateThreadMutex);
		s_UpdateThreadQueue.push(std::move(function));
	}

	void Application::RT_CreateContext()
	{
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
	}

	void Application::UT_MainLoop()
	{
		// Load resources
		{
			SplashScreen::SetProgress(70, "Loading resources...");

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(2000ms);

			Ref<Texture2D> texture = Ref<Texture2D>::Create("Resources/Splash/Splash.bmp");

			// load resources here
		}

		SplashScreen::Hide();
		SubmitToMainThread([]()
		{
			s_MainWindow->SetVisible(true);
		});

		float lastTime = static_cast<float>(glfwGetTime());
		uint32 frames = 0;

		while (s_Running)
		{
			Timer frameTimer(TimeUnit::Milliseconds);

			float time = static_cast<float>(glfwGetTime());
			frames++;
			if (time >= lastTime + 1.0f)
			{
				// LQ_INFO_ARGS("{0} fps", frames);
				s_FPS = frames;
				frames = 0;
				lastTime = time;
			}

			// Execute queue
			{
				std::unique_lock<std::mutex> lock(s_UpdateThreadMutex);
				while (!s_UpdateThreadQueue.empty())
				{
					auto& callback = s_UpdateThreadQueue.front();
					callback();
					s_UpdateThreadQueue.pop();
				}
			}

			if (!s_Minimized)
			{
				RT_SUBMIT(BeginFrame)([]()
				{
					s_Swapchain->BeginFrame();
					s_Swapchain->Clear(SwapchainBufferType::Color | SwapchainBufferType::Depth);
				});

				if (s_ImGuiRenderer)
				{
					RT_SUBMIT(RenderImGui)([]()
					{
						RenderImGui();
					});
				}

				RT_SUBMIT(Present)([]()
				{
					s_Swapchain->Present();
				});

				s_RenderThread->PushJob("Render Thread - Flush", []()
				{
					Timer timer;
					RenderThreadQueue::Flush();
					s_RenderThreadTime = timer.Elapsed();
				});
			}

			// Wait
			{
				Timer waitTimer(TimeUnit::Milliseconds);
				s_RenderThread->Wait();
				s_UpdateThreadWaitTime = waitTimer.Elapsed();
			}

			s_UpdateThreadTime = frameTimer.Elapsed();
		}

		SubmitToMainThread([]()
		{
			s_MainWindow->SetVisible(false);
		});
	}

	void Application::RenderImGui()
	{
		s_ImGuiRenderer->BeginFrame();

		s_ThemeBuilder->Render();

		ImGui::Begin("Liquid Engine");
		ImGui::Text("%d fps", s_FPS);

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

		if (ImGui::CollapsingHeader("Timers"))
		{
			ImGui::Text("Update Thread: %.4fms", s_UpdateThreadTime);
			ImGui::Text("Update Thread (Wait): %.4fms", s_UpdateThreadWaitTime);
			ImGui::Text("Render Thread: %.4fms", s_RenderThreadTime);
		}

		bool vsync = s_Swapchain->IsVSyncEnabled();
		if (ImGui::Checkbox("V-Sync", &vsync))
			s_Swapchain->SetVSync(vsync);

		ImGui::End();

		s_ImGuiRenderer->EndFrame();
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
		{
			s_Minimized = false;
			return;
		}

		s_RenderThread->PushJob("Swapchain Resize", [width, height, fullscreen = s_MainWindow->IsFullscreen()]()
		{
			s_Swapchain->Resize(width, height, fullscreen);
		});
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