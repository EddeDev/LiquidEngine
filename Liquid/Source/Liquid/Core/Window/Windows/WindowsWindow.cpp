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

		CenterWindow();
		CreateContext();
		CreateSwapchain();
		SetVisible(true);
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

	void WindowsWindow::SwapBuffers() const
	{
		m_Swapchain->Present();
	}

	void WindowsWindow::CreateContext()
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

	void WindowsWindow::CreateSwapchain()
	{
		SwapchainCreateInfo swapchainCreateInfo;
		swapchainCreateInfo.Context = m_Context;
		swapchainCreateInfo.WindowHandle = glfwGetWin32Window(m_Window);
		swapchainCreateInfo.InitialWidth = m_Data.FramebufferWidth;
		swapchainCreateInfo.InitialHeight = m_Data.FramebufferHeight;
		swapchainCreateInfo.ColorFormat = PixelFormat::RGBA;
		swapchainCreateInfo.DepthFormat = PixelFormat::DEPTH24_STENCIL8;
		swapchainCreateInfo.BufferCount = 3;
		swapchainCreateInfo.SampleCount = 1;
		m_Swapchain = Swapchain::Create(swapchainCreateInfo);
	}

	void WindowsWindow::CenterWindow()
	{
		auto videomode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int32 xpos = (videomode->width - m_Data.Width) / 2;
		int32 ypos = (videomode->height - m_Data.Height) / 2;
		glfwSetWindowPos(m_Window, xpos, ypos);
	}

	void WindowsWindow::SetVisible(bool visible)
	{
		if (visible)
			glfwShowWindow(m_Window);
		else
			glfwHideWindow(m_Window);
	}

}