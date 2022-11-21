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

		virtual void* GetPlatformHandle() const override;

		virtual void AddCloseCallback(const std::function<void()>& callback) override { m_Data.CloseCallbacks.push_back(callback); }
		virtual void AddWindowSizeCallback(const std::function<void(uint32, uint32)>& callback) override { m_Data.WindowSizeCallbacks.push_back(callback); }
		virtual void AddFramebufferSizeCallback(const std::function<void(uint32, uint32)>& callback) override { m_Data.FramebufferSizeCallbacks.push_back(callback); }

		virtual void SetVisible(bool visible) override;
		virtual bool IsVisible() const override;

		virtual void SetTitle(const String& title) override;
		virtual const String& GetTitle() const override { return m_Data.Title; }

		virtual uint32 GetWidth() const override { return m_Data.Width; }
		virtual uint32 GetHeight() const override { return m_Data.Height; }

		virtual uint32 GetFramebufferWidth() const override { return m_Data.FramebufferWidth; }
		virtual uint32 GetFramebufferHeight() const override { return m_Data.FramebufferHeight; }
	private:
		void Init(const WindowCreateInfo& createInfo);
		void Shutdown();
	private:
		struct WindowData
		{
			uint32 Width, Height;
			uint32 FramebufferWidth, FramebufferHeight;
			String Title;

			std::vector<std::function<void()>> CloseCallbacks;
			std::vector<std::function<void(int32, uint32)>> WindowSizeCallbacks;
			std::vector<std::function<void(uint32, uint32)>> FramebufferSizeCallbacks;
		} m_Data;

		GLFWwindow* m_Window = nullptr;
	};

}