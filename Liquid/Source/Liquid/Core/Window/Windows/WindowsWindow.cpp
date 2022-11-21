#include "LiquidPCH.h"
#include "WindowsWindow.h"

#include <GLFW/glfw3.h>
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
		m_Data.Width = createInfo.Width;
		m_Data.Height = createInfo.Height;
		m_Data.Title = createInfo.Title;

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
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		LQ_VERIFY(m_Window, "Failed to create GLFW window");
		s_GLFWWindowCount++;

		// Get window size
		int32 width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		m_Data.Width = width;
		m_Data.Height = height;

		// Get framebuffer size
		int32 framebufferWidth, framebufferHeight;
		glfwGetFramebufferSize(m_Window, &framebufferWidth, &framebufferHeight);
		m_Data.FramebufferWidth = framebufferWidth;
		m_Data.FramebufferHeight = framebufferHeight;

		// Center window
		auto videomode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int32 xpos = (videomode->width - m_Data.Width) / 2;
		int32 ypos = (videomode->height - m_Data.Height) / 2;
		glfwSetWindowPos(m_Window, xpos, ypos);

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
			data.Width = width;
			data.Height = height;
			for (auto& callback : data.WindowSizeCallbacks)
				callback(width, height);
		});

		glfwSetFramebufferSizeCallback(m_Window, [](auto window, auto width, auto height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.FramebufferWidth = width;
			data.FramebufferHeight = height;
			for (auto& callback : data.FramebufferSizeCallbacks)
				callback(width, height);
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

}