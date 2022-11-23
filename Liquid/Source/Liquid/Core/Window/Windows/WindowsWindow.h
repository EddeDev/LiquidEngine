#pragma once

#include "Liquid/Core/Window/Window.h"

struct GLFWwindow;
struct GLFWmonitor;

namespace Liquid {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowCreateInfo& createInfo);
		virtual ~WindowsWindow();

		virtual void PollEvents() const override;

		virtual void* GetHandle() const override { return m_Window; }
		virtual void* GetPlatformHandle() const override;

		virtual void AddCloseCallback(const std::function<void()>& callback) override { m_Data.CloseCallbacks.push_back(callback); }
		virtual void AddWindowSizeCallback(const std::function<void(uint32, uint32)>& callback) override { m_Data.WindowSizeCallbacks.push_back(callback); }

		virtual void SetVisible(bool visible) override;
		virtual bool IsVisible() const override;

		virtual void SetTitle(const String& title) override;
		virtual const String& GetTitle() const override { return m_Data.Title; }

		virtual void SetFullscreen(bool enabled) override;
		virtual bool IsFullscreen() const override { return m_Data.Fullscreen; }

		virtual uint32 GetWidth() const override { return m_Data.Backups[m_Data.CurrentBackupIndex].Width; }
		virtual uint32 GetHeight() const override { return m_Data.Backups[m_Data.CurrentBackupIndex].Height; }
	private:
		void Init(const WindowCreateInfo& createInfo);
		void Shutdown();
	private:
		struct WindowBackupData
		{
			uint32 Width, Height;
			uint32 XPos, YPos;
		};

		struct WindowData
		{
			std::array<WindowBackupData, 2> Backups;
			uint32 CurrentBackupIndex = 0;
			String Title;
			bool Fullscreen;

			std::vector<std::function<void()>> CloseCallbacks;
			std::vector<std::function<void(int32, uint32)>> WindowSizeCallbacks;
		} m_Data;

		GLFWwindow* m_Window = nullptr;
		GLFWmonitor* m_Monitor = nullptr;
	};

}