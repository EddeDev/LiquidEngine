#pragma once

#include "Liquid/Launch/CommandLineArgs.h"

#include "Liquid/Renderer/API/GraphicsAPI.h"
#include "Liquid/Renderer/API/GraphicsDevice.h"
#include "Liquid/Renderer/API/GraphicsContext.h"
#include "Liquid/Renderer/API/Swapchain.h"

#include "Liquid/Renderer/ImGuiRenderer.h"

#include "Window/Window.h"
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
	};

	class Application
	{
	public:
		using DelayedCallback = std::function<void()>;
	public:
		static void Init(const ApplicationCreateInfo& createInfo);
		static void Shutdown();

		static void Run();
		static void PushDelayedCallback(DelayedCallback callback);

		static BuildConfiguration GetBuildConfiguration();
		static GraphicsAPI GetGraphicsAPI();

		static Ref<Window> GetMainWindow() { return s_MainWindow; }
		static Ref<GraphicsDevice> GetDevice() { return s_Device; }
		static Ref<GraphicsContext> GetContext() { return s_Context; }
		static Ref<Swapchain> GetSwapchain() { return s_Swapchain; }
	private:
		static void UpdateThreadLoop(bool singlethreaded);

		static void OnWindowCloseCallback();
		static void OnWindowSizeCallback(uint32 width, uint32 height);
	private:
		static Ref<Window> s_MainWindow;
		static Ref<GraphicsDevice> s_Device;
		static Ref<GraphicsContext> s_Context;
		static Ref<Swapchain> s_Swapchain;
		static Ref<ImGuiRenderer> s_ImGuiRenderer;
		static Unique<ThemeBuilder> s_ThemeBuilder;

		static std::queue<DelayedCallback> s_DelayedCallbacks;
		static std::mutex s_DelayedCallbackMutex;

		static std::atomic<bool> s_Running;
		static std::atomic<bool> s_Minimized;
	};

}