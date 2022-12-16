#include "LiquidPCH.h"
#include "WindowsWindow.h"

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Liquid {

	static uint32 s_GLFWWindowCount = 0;

	WindowsWindow::WindowsWindow(const WindowCreateInfo& createInfo)
	{
		Init(createInfo);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	} 

	void WindowsWindow::Init(const WindowCreateInfo& createInfo)
	{
		m_ThreadID = std::this_thread::get_id();

		m_Data.CurrentBackupIndex = createInfo.Fullscreen ? 1 : 0;
		m_Data.Title = createInfo.Title;
		m_Data.Fullscreen = createInfo.Fullscreen;

		if (s_GLFWWindowCount == 0)
		{
			LQ_INFO_ARGS("Initializing GLFW...");
			int32 initialized = glfwInit();
			LQ_VERIFY(initialized, "Could not initialize GLFW");

			int32 major, minor, revision;
			glfwGetVersion(&major, &minor, &revision);
			LQ_INFO_ARGS("GLFW version: {0}.{1}.{2}", major, minor, revision);
		}

		LQ_TRACE_CATEGORY("Window", "Monitors:");
		int32 monitorCount;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
		for (int32 i = 0; i < monitorCount; i++)
		{
			GLFWmonitor* monitor = monitors[i];
			const char* monitorName = glfwGetMonitorName(monitor);
			LQ_TRACE_CATEGORY("Window", "[{0}]: {1}{2}", i, monitorName, monitor == glfwGetPrimaryMonitor() ? " (Primary)" : "");
		}

		m_Monitor = glfwGetPrimaryMonitor();

		auto videomode = glfwGetVideoMode(m_Monitor);
		m_Data.Backups[0].Width = createInfo.Width;
		m_Data.Backups[0].Height = createInfo.Height;
		m_Data.Backups[0].XPos = 0;
		m_Data.Backups[0].YPos = 0;
		m_Data.Backups[1].Width = videomode->width;
		m_Data.Backups[1].Height = videomode->height;
		m_Data.Backups[1].XPos = 0;
		m_Data.Backups[1].YPos = 0;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_DEPTH_BITS, 0);
		glfwWindowHint(GLFW_STENCIL_BITS, 0);
		glfwWindowHint(GLFW_ALPHA_BITS, 0);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, createInfo.Resizable);
		glfwWindowHint(GLFW_FLOATING, m_Data.Fullscreen);
		glfwWindowHint(GLFW_DECORATED, createInfo.Decorated && !m_Data.Fullscreen);
		glfwWindowHint(GLFW_MAXIMIZED, createInfo.Maximize);

		auto& currentBackup = m_Data.Backups[m_Data.CurrentBackupIndex];

		m_Window = glfwCreateWindow(currentBackup.Width, currentBackup.Height, m_Data.Title.c_str(), m_Data.Fullscreen ? m_Monitor : nullptr, nullptr);
		LQ_VERIFY(m_Window, "Failed to create GLFW window");
		s_GLFWWindowCount++;

		// Get window size
		int32 width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		currentBackup.Width = width;
		currentBackup.Height = height;

		int32 isMaximized = glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED);
		if (!m_Data.Fullscreen && !isMaximized)
		{
			// Center window
			currentBackup.XPos = (videomode->width - currentBackup.Width) / 2;
			currentBackup.YPos = (videomode->height - currentBackup.Height) / 2;
			glfwSetWindowPos(m_Window, currentBackup.XPos, currentBackup.YPos);
		}

		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetWindowFocusCallback(m_Window, [](auto window, int32 focused)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.FocusCallbacks)
				callback(focused);
		});

		glfwSetCursorEnterCallback(m_Window, [](auto window, int32 entered)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.FocusCallbacks)
				callback(entered);
		});

		glfwSetCursorPosCallback(m_Window, [](auto window, double xpos, double ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.CursorPosCallbacks)
				callback(xpos, ypos);
		});

		glfwSetMouseButtonCallback(m_Window, [](auto window, int32 button, int32 action, int32 mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.MouseButtonCallbacks)
				callback(button, action, mods);
		});

		glfwSetScrollCallback(m_Window, [](auto window, double xoffset, double yoffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.ScrollCallbacks)
				callback(xoffset, yoffset);
		});

		glfwSetKeyCallback(m_Window, [](auto window, int32 key, int32 scancode, int32 action, int32 mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.KeyCallbacks)
				callback(key, scancode, action, mods);
		});

		glfwSetCharCallback(m_Window, [](auto window, uint32 codepoint)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.CharCallbacks)
				callback(codepoint);
		});

		glfwSetWindowCloseCallback(m_Window, [](auto window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			for (auto& callback : data.CloseCallbacks)
				callback();
		});

		glfwSetWindowSizeCallback(m_Window, [](auto window, auto width, auto height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Backups[data.CurrentBackupIndex].Width = width;
			data.Backups[data.CurrentBackupIndex].Height = height;
			for (auto& callback : data.WindowSizeCallbacks)
				callback(width, height);
		});

		glfwSetWindowPosCallback(m_Window, [](auto window, auto xpos, auto ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Backups[data.CurrentBackupIndex].XPos = xpos;
			data.Backups[data.CurrentBackupIndex].YPos = ypos;
		});
	}

	void WindowsWindow::Shutdown()
	{
		LQ_ASSERT(std::this_thread::get_id() == m_ThreadID, "Window::Shutdown must only be called from the main thread");

		glfwDestroyWindow(m_Window);
		s_GLFWWindowCount--;

		if (s_GLFWWindowCount == 0)
			glfwTerminate();
	}

	void WindowsWindow::CreateContext()
	{
		m_ContextThreadID = std::this_thread::get_id();
		if (m_ThreadID == m_ContextThreadID)
			LQ_WARNING_ARGS("Window and graphics context was created on the same thread.");

		// Context
		{
			GraphicsContextCreateInfo contextCreateInfo;
			contextCreateInfo.WindowHandle = glfwGetWin32Window(m_Window);
#ifdef LQ_BUILD_DEBUG
			contextCreateInfo.EnableDebugLayers = true;
#else
			contextCreateInfo.EnableDebugLayers = false;
#endif

			m_Context = GraphicsContext::Create(contextCreateInfo);
		}

		// Swapchain
		{
			SwapchainCreateInfo swapchainCreateInfo;
			swapchainCreateInfo.WindowHandle = glfwGetWin32Window(m_Window);
			swapchainCreateInfo.InitialWidth = m_Data.Backups[m_Data.CurrentBackupIndex].Width;
			swapchainCreateInfo.InitialHeight = m_Data.Backups[m_Data.CurrentBackupIndex].Height;
			swapchainCreateInfo.InitialFullscreenState = m_Data.Fullscreen;
			swapchainCreateInfo.AllowTearing = true;
			swapchainCreateInfo.ColorFormat = PixelFormat::RGBA;
			swapchainCreateInfo.DepthFormat = PixelFormat::DEPTH24_STENCIL8;
			swapchainCreateInfo.BufferCount = 3;
			swapchainCreateInfo.SampleCount = 1;

			m_Swapchain = Swapchain::Create(swapchainCreateInfo);
		}
	}

	void WindowsWindow::BeginFrame()
	{
		m_Swapchain->BeginFrame();
		m_Swapchain->Clear(BUFFER_COLOR | BUFFER_DEPTH);
	}

	void WindowsWindow::SwapBuffers()
	{
		m_Swapchain->Present();
	}

	void WindowsWindow::PollEvents() const
	{
		LQ_ASSERT(std::this_thread::get_id() == m_ThreadID, "Window::PollEvents must only be called from the main thread");

		glfwPollEvents();
	}

	void WindowsWindow::WaitEvents() const
	{
		LQ_ASSERT(std::this_thread::get_id() == m_ThreadID, "Window::WaitEvents must only be called from the main thread");

		glfwWaitEvents();
	}

	void WindowsWindow::PostEmptyEvent() const
	{
		glfwPostEmptyEvent();
	}

	void* WindowsWindow::GetPlatformHandle() const
	{
		return glfwGetWin32Window(m_Window);
	}

	void WindowsWindow::SetVisible(bool visible)
	{
		LQ_ASSERT(std::this_thread::get_id() == m_ThreadID, "Window::SetVisible must only be called from the main thread");

		if (visible)
			glfwShowWindow(m_Window);
		else
			glfwHideWindow(m_Window);
	}

	bool WindowsWindow::IsVisible() const
	{
		LQ_ASSERT(std::this_thread::get_id() == m_ThreadID, "Window::IsVisible must only be called from the main thread");

		return glfwGetWindowAttrib(m_Window, GLFW_VISIBLE);
	}

	void WindowsWindow::SetTitle(const String& title)
	{
		LQ_ASSERT(std::this_thread::get_id() == m_ThreadID, "Window::SetTitle must only be called from the main thread");

		if (m_Data.Title != title)
		{
			glfwSetWindowTitle(m_Window, title.c_str());
			m_Data.Title = title;
		}
	}

	void WindowsWindow::SetFullscreen(bool enabled)
	{
		LQ_ASSERT(std::this_thread::get_id() == m_ThreadID, "Window::SetFullscreen must only be called from the main thread");

		if (m_Data.Fullscreen != enabled)
		{
			m_Data.Fullscreen = enabled;
			m_Data.CurrentBackupIndex = enabled ? 1 : 0;

			auto& currentBackup = m_Data.Backups[m_Data.CurrentBackupIndex];
			glfwSetWindowMonitor(m_Window, enabled ? m_Monitor : nullptr, currentBackup.XPos, currentBackup.YPos, currentBackup.Width, currentBackup.Height, GLFW_DONT_CARE);
		}
	}

}