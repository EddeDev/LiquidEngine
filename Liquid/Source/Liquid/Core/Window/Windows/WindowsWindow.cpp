#include "LiquidPCH.h"
#include "WindowsWindow.h"

#include <GLFW/glfw3.h>

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

		int32 width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		m_Data.Width = width;
		m_Data.Height = height;

		auto videomode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(m_Window, (videomode->width - width) / 2, (videomode->height - height) / 2);

		glfwShowWindow(m_Window);
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

}