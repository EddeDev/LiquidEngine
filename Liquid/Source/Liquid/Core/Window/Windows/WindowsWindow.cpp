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
			int32 initialized = glfwInit();
			LQ_VERIFY(initialized, "Could not initialize GLFW");
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

	bool WindowsWindow::IsCloseRequested() const
	{
		return glfwWindowShouldClose(m_Window);
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