#include "LiquidPCH.h"
#include "Engine.h"

#include "Liquid/Renderer/RenderThread.h"
#include "Liquid/Renderer/ImGuiRenderer.h"

#include <glfw/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>

#include "Debugging/Timer.h"
#include "Debugging/ScopedTimer.h"

namespace Liquid {

	Engine::Engine(const EngineCreateInfo& createInfo)
	{
		LQ_CHECK(!s_Instance);
		s_Instance = this;

		m_MainThreadID = std::this_thread::get_id();

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

			SplashScreen::SetProgress(10, "Choosing graphics device...");
			m_Device = GraphicsDevice::Select(deviceCreateInfo);
		}

		// Window
		{
			WindowCreateInfo windowCreateInfo;
			windowCreateInfo.Width = createInfo.WindowWidth;
			windowCreateInfo.Height = createInfo.WindowHeight;
			windowCreateInfo.Title = createInfo.WindowTitle;
			windowCreateInfo.Fullscreen = createInfo.Fullscreen;
			windowCreateInfo.Maximize = createInfo.MaximizeOnStart;

			SplashScreen::SetProgress(20, "Creating window...");
			m_MainWindow = Window::Create(windowCreateInfo);
			m_MainWindow->AddCloseCallback(LQ_BIND_CALLBACK(OnWindowCloseCallback, this));
			m_MainWindow->AddWindowSizeCallback(LQ_BIND_CALLBACK(OnWindowSizeCallback, this));
		}

		// ImGui
		if (createInfo.EnableImGui)
		{
			ImGuiRendererCreateInfo createInfo;
			createInfo.Window = m_MainWindow;
			createInfo.DebugName = "ImGuiRenderer-Main";
			createInfo.ViewportsEnable = false;

			SplashScreen::SetProgress(30, "Creating ImGui context...");
			m_ImGuiRenderer = Ref<ImGuiRenderer>::Create(createInfo);
		}
	}

	Engine::~Engine()
	{
		RenderThreadQueue::Shutdown();
	}

	void Engine::Run()
	{
		// Render Thread
		ThreadCreateInfo renderThreadCreateInfo;
		renderThreadCreateInfo.Name = "Render Thread";
		renderThreadCreateInfo.Priority = ThreadPriority::Highest;
		m_RenderThread = CreateUnique<Thread>(renderThreadCreateInfo);
		m_RenderThread->PushJob("Render Thread - Create Context", [this]()
		{
			m_MainWindow->CreateContext();
		});

		// Update Thread
		ThreadCreateInfo updateThreadCreateInfo;
		updateThreadCreateInfo.Name = "Update Thread";
		updateThreadCreateInfo.Priority = ThreadPriority::AboveNormal;
		m_UpdateThread = CreateUnique<Thread>(updateThreadCreateInfo);
		m_UpdateThread->PushJob("Update Thread - Main Loop", LQ_BIND_CALLBACK(UT_MainLoop, this));

		while (m_Running)
		{
			m_MainWindow->WaitEvents();

			// Execute queue
			{
				std::unique_lock<std::mutex> lock(m_MainThreadMutex);
				while (!m_MainThreadQueue.empty())
				{
					auto& callback = m_MainThreadQueue.front();
					callback();
					m_MainThreadQueue.pop();
				}
			}
		}

		m_UpdateThread->Wait();
		m_RenderThread->Wait();

		m_UpdateThread.reset();
		m_RenderThread.reset();
	}

#ifdef LQ_BUILD_DEBUG
#define THREAD_CHECKS 1
#else
#define THREAD_CHECKS 0
#endif

	void Engine::SubmitToMainThread(std::function<void()> function)
	{
#if THREAD_CHECKS
		if (std::this_thread::get_id() == m_MainThreadID)
			LQ_WARNING_ARGS("Application::SubmitToMainThread was called from the main thread");
		if (std::this_thread::get_id() == m_RenderThread->GetThreadID())
			LQ_WARNING_ARGS("Application::SubmitToMainThread was called from the render thread");
#endif

		std::lock_guard<std::mutex> lock(m_MainThreadMutex);
		m_MainThreadQueue.push(std::move(function));
		m_MainWindow->PostEmptyEvent();
	}

	void Engine::SubmitToUpdateThread(std::function<void()> function)
	{
#if THREAD_CHECKS
		if (std::this_thread::get_id() == m_UpdateThread->GetThreadID())
			LQ_WARNING_ARGS("Application::SubmitToUpdateThread was called from the update thread");
		if (std::this_thread::get_id() == m_RenderThread->GetThreadID())
			LQ_WARNING_ARGS("Application::SubmitToUpdateThread was called from the render thread");
#endif

		std::lock_guard<std::mutex> lock(m_UpdateThreadMutex);
		m_UpdateThreadQueue.push(std::move(function));
	}

	void Engine::UT_MainLoop()
	{
		OnInit();

		SplashScreen::Hide();
		SubmitToMainThread([this]()
		{
			m_MainWindow->SetVisible(true);
		});

		float lastTime = static_cast<float>(glfwGetTime());
		uint32 frames = 0;

		while (m_Running)
		{
			Timer frameTimer(TimeUnit::Milliseconds);

			// Execute queue
			{
				std::unique_lock<std::mutex> lock(m_UpdateThreadMutex);
				while (!m_UpdateThreadQueue.empty())
				{
					auto& callback = m_UpdateThreadQueue.front();
					callback();
					m_UpdateThreadQueue.pop();
				}
			}

			if (!m_Minimized)
			{
				RT_SUBMIT(BeginFrame)([this]()
				{
					m_MainWindow->BeginFrame();
				});

				OnUpdate();

				if (m_ImGuiRenderer)
				{
					RT_SUBMIT(RenderImGui)([this]()
					{
						m_ImGuiRenderer->BeginFrame();
						OnUIRender();
						m_ImGuiRenderer->EndFrame();
					});
				}

				RT_SUBMIT(SwapBuffers)([this]()
				{
					m_MainWindow->SwapBuffers();
				});

				m_RenderThread->PushJob("Render Thread - Flush", RenderThreadQueue::Flush);
			}

			// Wait
			m_RenderThread->Wait();
		}

		OnShutdown();

		SubmitToMainThread([this]()
		{
			m_MainWindow->SetVisible(false);
		});
	}

	void Engine::OnWindowCloseCallback()
	{
		m_Running = false;
	}

	void Engine::OnWindowSizeCallback(uint32 width, uint32 height)
	{
		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return;
		}

		if (m_Minimized)
		{
			m_Minimized = false;
			return;
		}

		m_RenderThread->PushJob("Swapchain Resize", [this, width, height, fullscreen = m_MainWindow->IsFullscreen()]()
		{
			Ref<Swapchain> swapchain = m_MainWindow->GetSwapchain();
			swapchain->Resize(width, height, fullscreen);
		});
	}

	BuildConfiguration Engine::GetBuildConfiguration() const
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

	GraphicsAPI Engine::GetGraphicsAPI() const
	{
		return GraphicsAPI::DX11;
	}

}