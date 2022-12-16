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

	struct EngineCreateInfo
	{
		CommandLineArgs Args;
		uint32 WindowWidth = 1280;
		uint32 WindowHeight = 720;
		bool EnableImGui = true;
		bool ShowSplashScreen = true;
		bool MaximizeOnStart = true;
		bool Fullscreen = false;
	};

	class Engine
	{
	public:
		Engine(const EngineCreateInfo& createInfo);
		~Engine();

		void Run();
		void SubmitToMainThread(std::function<void()> function);
		void SubmitToUpdateThread(std::function<void()> function);

		BuildConfiguration GetBuildConfiguration() const;
		GraphicsAPI GetGraphicsAPI() const;

		Ref<Window> GetMainWindow() const { return m_MainWindow; }
		Ref<GraphicsDevice> GetDevice() const { return m_Device; }

		const Unique<Thread>& GetUpdateThread() const { return m_UpdateThread; }
		const Unique<Thread>& GetRenderThread() const { return m_UpdateThread; }

		static Engine& Get() { return *s_Instance; }
	protected:
		virtual void OnInit();
		virtual void OnShutdown();
		virtual void OnUpdate();
		virtual void OnUIRender();
	private:
		void UT_MainLoop();

		void OnWindowCloseCallback();
		void OnWindowSizeCallback(uint32 width, uint32 height);
	private:
		inline static Engine* s_Instance = nullptr;

		Ref<Window> m_MainWindow;
		Ref<GraphicsDevice> m_Device;
		Ref<ImGuiRenderer> m_ImGuiRenderer;

		std::queue<std::function<void()>> m_MainThreadQueue;
		std::queue<std::function<void()>> m_UpdateThreadQueue;
		std::mutex m_MainThreadMutex;
		std::mutex m_UpdateThreadMutex;
		std::thread::id m_MainThreadID;

		Unique<Thread> m_UpdateThread;
		Unique<Thread> m_RenderThread;

		std::atomic<bool> m_Running = true;
		std::atomic<bool> m_Minimized = false;
	};

}

#define LQ_CURRENT_GRAPHICS_API ::Liquid::Engine::Get().GetGraphicsAPI()