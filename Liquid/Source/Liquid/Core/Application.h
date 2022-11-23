#pragma once

#include "Liquid/Launch/CommandLineArgs.h"

#include "Liquid/Renderer/API/GraphicsAPI.h"
#include "Liquid/Renderer/API/GraphicsContext.h"
#include "Liquid/Renderer/API/Swapchain.h"

#include "Window/Window.h"

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
		static void Init(const ApplicationCreateInfo& createInfo);
		static void Shutdown();

		static void Run();

		static BuildConfiguration GetBuildConfiguration();
		static GraphicsAPI GetGraphicsAPI();

		static Ref<Window> GetWindow() { return s_Window; }
		static Ref<GraphicsContext> GetContext() { return s_Context; }
		static Ref<Swapchain> GetSwapchain() { return s_Swapchain; }
	private:
		static void OnWindowCloseCallback();
		static void OnWindowSizeCallback(uint32 width, uint32 height);
		static void OnFramebufferSizeCallback(uint32 width, uint32 height);
	private:
		static Ref<Window> s_Window;
		static Ref<GraphicsContext> s_Context;
		static Ref<Swapchain> s_Swapchain;
		static bool s_Running;
		static bool s_Minimized;
	};

}