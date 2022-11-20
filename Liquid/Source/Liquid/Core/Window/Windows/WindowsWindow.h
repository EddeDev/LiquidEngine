#pragma once

#include "Liquid/Core/Window/Window.h"

#include "Liquid/Renderer/API/GraphicsContext.h"
#include "Liquid/Renderer/API/Swapchain.h"

struct GLFWwindow;

namespace Liquid {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowCreateInfo& createInfo);
		virtual ~WindowsWindow();

		virtual void PollEvents() const override;
		virtual void SwapBuffers() const override;

		virtual Ref<GraphicsContext> GetContext() const override { return m_Context; }
		virtual Ref<Swapchain> GetSwapchain() const override { return m_Swapchain; }
	private:
		void Init(const WindowCreateInfo& createInfo);
		void Shutdown();

		void CreateContext();
		void CreateSwapchain();

		void CenterWindow();
		void SetVisible(bool visible);
	private:
		struct WindowData
		{
			uint32 Width, Height;
			uint32 FramebufferWidth, FramebufferHeight;
			String Title;
		} m_Data;

		GLFWwindow* m_Window = nullptr;

		Ref<GraphicsContext> m_Context;
		Ref<Swapchain> m_Swapchain;
	};

}