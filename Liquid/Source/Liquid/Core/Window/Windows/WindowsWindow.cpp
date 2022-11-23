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

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_DEPTH_BITS, 0);
		glfwWindowHint(GLFW_STENCIL_BITS, 0);
		glfwWindowHint(GLFW_ALPHA_BITS, 0);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_DECORATED, createInfo.Decorated);

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

		auto& currentBackup = m_Data.Backups[m_Data.CurrentBackupIndex];

		if (m_Data.Fullscreen)
		{
			glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

			m_Window = glfwCreateWindow(currentBackup.Width, currentBackup.Height, m_Data.Title.c_str(), m_Monitor, nullptr);
		}
		else
		{
			m_Window = glfwCreateWindow(currentBackup.Width, currentBackup.Height, m_Data.Title.c_str(), nullptr, nullptr);
		
			// Center window
			currentBackup.XPos = (videomode->width - currentBackup.Width) / 2;
			currentBackup.YPos = (videomode->height - currentBackup.Height) / 2;
			glfwSetWindowPos(m_Window, currentBackup.XPos, currentBackup.YPos);
		}
		LQ_VERIFY(m_Window, "Failed to create GLFW window");
		s_GLFWWindowCount++;

		// Get window size
		int32 width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		currentBackup.Width = width;
		currentBackup.Height = height;

		glfwSetWindowUserPointer(m_Window, &m_Data);

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
		glfwDestroyWindow(m_Window);
		s_GLFWWindowCount--;

		if (s_GLFWWindowCount == 0)
			glfwTerminate();
	}

	void WindowsWindow::PollEvents() const
	{
		glfwPollEvents();
	}

	void* WindowsWindow::GetPlatformHandle() const
	{
		return glfwGetWin32Window(m_Window);
	}

	void WindowsWindow::SetVisible(bool visible)
	{
		if (visible)
			glfwShowWindow(m_Window);
		else
			glfwHideWindow(m_Window);
	}

	bool WindowsWindow::IsVisible() const
	{
		return glfwGetWindowAttrib(m_Window, GLFW_VISIBLE);
	}

	void WindowsWindow::SetTitle(const String& title)
	{
		if (m_Data.Title != title)
		{
			glfwSetWindowTitle(m_Window, title.c_str());
			m_Data.Title = title;
		}
	}

	void WindowsWindow::SetFullscreen(bool enabled)
	{
		if (m_Data.Fullscreen != enabled)
		{
			m_Data.Fullscreen = enabled;
			m_Data.CurrentBackupIndex = enabled ? 1 : 0;

			auto& currentBackup = m_Data.Backups[m_Data.CurrentBackupIndex];
			glfwSetWindowMonitor(m_Window, enabled ? m_Monitor : nullptr, currentBackup.XPos, currentBackup.YPos, currentBackup.Width, currentBackup.Height, GLFW_DONT_CARE);
		}
	}

}