#pragma once

#include "Liquid/Launch/CommandLineArgs.h"

#include "Liquid/Renderer/API/GraphicsAPI.h"
#include "Liquid/Renderer/API/GraphicsDevice.h"
#include "Liquid/Renderer/API/GraphicsContext.h"
#include "Liquid/Renderer/API/Swapchain.h"

#include "Liquid/Renderer/ImGuiRenderer.h"
#include "Liquid/Renderer/Texture.h"

#include "Threading/Thread.h"
#include "Window/Window.h"
#include "SplashScreen/SplashScreen.h"
#include "ThemeBuilder.h"

namespace Liquid {

	enum class BuildConfiguration : uint8
	{
		None = 0,

		Debug,
		Release,
		Shipping
	};

	enum class BuildTargetType : uint8
	{
		None = 0,

		Game,
		Editor
	};

	struct ApplicationCreateInfo
	{
		CommandLineArgs Args;
		bool EnableImGui = true;
		bool ShowSplashScreen = true;
	};

	class Application
	{
	public:
		static void Init(const ApplicationCreateInfo& createInfo);
		static void Shutdown();

		static void Run();
		static void SubmitToMainThread(std::function<void()> function);
		static void SubmitToUpdateThread(std::function<void()> function);

		static BuildConfiguration GetBuildConfiguration();
		static GraphicsAPI GetGraphicsAPI();

		static Ref<Window> GetMainWindow() { return s_MainWindow; }
		static Ref<GraphicsDevice> GetDevice() { return s_Device; }
		static Ref<GraphicsContext> GetContext() { return s_Context; }
		static Ref<Swapchain> GetSwapchain() { return s_Swapchain; }

		static const Unique<Thread>& GetUpdateThread() { return s_UpdateThread; }
		static const Unique<Thread>& GetRenderThread() { return s_UpdateThread; }
	private:
		static void RT_CreateContext();
		static void UT_MainLoop();

		static void RenderImGui();

		static void OnWindowCloseCallback();
		static void OnWindowSizeCallback(uint32 width, uint32 height);
	private:
		static Ref<Window> s_MainWindow;
		static Ref<GraphicsDevice> s_Device;
		static Ref<GraphicsContext> s_Context;
		static Ref<Swapchain> s_Swapchain;
		static Ref<ImGuiRenderer> s_ImGuiRenderer;
		static Ref<Texture2D> s_TestTexture;
		static Unique<ThemeBuilder> s_ThemeBuilder;

		static std::queue<std::function<void()>> s_MainThreadQueue;
		static std::queue<std::function<void()>> s_UpdateThreadQueue;
		static std::mutex s_MainThreadMutex;
		static std::mutex s_UpdateThreadMutex;
		static std::thread::id s_MainThreadID;

		static Unique<Thread> s_UpdateThread;
		static Unique<Thread> s_RenderThread;

		static std::atomic<bool> s_Running;
		static std::atomic<bool> s_Minimized;

		static uint32 s_FPS;

		static float s_UpdateThreadTime;
		static float s_UpdateThreadWaitTime;
		static float s_RenderThreadTime;
	};

}