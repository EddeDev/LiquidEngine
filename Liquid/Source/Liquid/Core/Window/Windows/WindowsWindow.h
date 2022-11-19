#pragma once

#include "Liquid/Core/Window/Window.h"

struct GLFWwindow;

namespace Liquid {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowCreateInfo& createInfo);
		virtual ~WindowsWindow();

		virtual void PollEvents() const override;
	private:
		void Init(const WindowCreateInfo& createInfo);
		void Shutdown();
	private:
		struct WindowData
		{
			uint32 Width, Height;
			String Title;
		} m_Data;

		GLFWwindow* m_Window = nullptr;
	};

}