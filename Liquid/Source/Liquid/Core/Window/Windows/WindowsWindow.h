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
		virtual void WaitEvents() const override;

		virtual void* GetHandle() const override { return m_Window; }
		virtual void* GetPlatformHandle() const override;

		virtual void AddFocusCallback(const std::function<void(int32)>& callback) override { m_Data.FocusCallbacks.push_back(callback); }
		virtual void AddCursorEnterCallback(const std::function<void(int32)>& callback) override { m_Data.CursorEnterCallbacks.push_back(callback); }
		virtual void AddCursorPosCallback(const std::function<void(double, double)>& callback) override { m_Data.CursorPosCallbacks.push_back(callback); }
		virtual void AddMouseButtonCallback(const std::function<void(int32, int32, int32)>& callback) override { m_Data.MouseButtonCallbacks.push_back(callback); }
		virtual void AddScrollCallback(const std::function<void(double, double)>& callback) override { m_Data.ScrollCallbacks.push_back(callback); }
		virtual void AddKeyCallback(const std::function<void(int32, int32, int32, int32)>& callback) override { m_Data.KeyCallbacks.push_back(callback); }
		virtual void AddCharCallback(const std::function<void(uint32)>& callback) override { m_Data.CharCallbacks.push_back(callback); }
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

			std::vector<std::function<void(int32)>> FocusCallbacks;
			std::vector<std::function<void(int32)>> CursorEnterCallbacks;
			std::vector<std::function<void(double, double)>> CursorPosCallbacks;
			std::vector<std::function<void(int32, int32, int32)>> MouseButtonCallbacks;
			std::vector<std::function<void(double, double)>> ScrollCallbacks;
			std::vector<std::function<void(int32, int32, int32, int32)>> KeyCallbacks;
			std::vector<std::function<void(uint32)>> CharCallbacks;
			std::vector<std::function<void()>> CloseCallbacks;
			std::vector<std::function<void(int32, uint32)>> WindowSizeCallbacks;
		} m_Data;

		GLFWwindow* m_Window = nullptr;
		GLFWmonitor* m_Monitor = nullptr;

		static std::mutex s_Mutex;
	};

}