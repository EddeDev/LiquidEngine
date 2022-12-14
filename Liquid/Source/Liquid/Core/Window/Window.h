#pragma once

#include "Liquid/Renderer/API/GraphicsContext.h"
#include "Liquid/Renderer/API/Swapchain.h"

namespace Liquid {

	struct WindowCreateInfo
	{
		uint32 Width = 1280;
		uint32 Height = 720;
		String Title = "Liquid Engine";
		bool Fullscreen = false;
		bool Maximize = false;
		bool Resizable = true;
		bool Decorated = true;
	};

	class Window : public RefCounted
	{
	public:
		virtual ~Window() {}

		virtual void CreateContext() = 0;
		virtual void BeginFrame() = 0;
		virtual void SwapBuffers() = 0;

		virtual void PollEvents() const = 0;
		virtual void WaitEvents() const = 0;
		virtual void PostEmptyEvent() const = 0;

		virtual void* GetHandle() const = 0;
		virtual void* GetPlatformHandle() const = 0;

		virtual void AddFocusCallback(const std::function<void(int32)>& callback) = 0;
		virtual void AddCursorEnterCallback(const std::function<void(int32)>& callback) = 0;
		virtual void AddCursorPosCallback(const std::function<void(double, double)>& callback) = 0;
		virtual void AddMouseButtonCallback(const std::function<void(int32, int32, int32)>& callback) = 0;
		virtual void AddScrollCallback(const std::function<void(double, double)>& callback) = 0;
		virtual void AddKeyCallback(const std::function<void(int32, int32, int32, int32)>& callback) = 0;
		virtual void AddCharCallback(const std::function<void(uint32)>& callback) = 0;
		virtual void AddCloseCallback(const std::function<void()>& callback) = 0;
		virtual void AddWindowSizeCallback(const std::function<void(uint32, uint32)>& callback) = 0;

		virtual void SetVisible(bool visible) = 0;
		virtual bool IsVisible() const = 0;

		virtual void SetTitle(const String& title) = 0;
		virtual const String& GetTitle() const = 0;

		virtual void SetFullscreen(bool enabled) = 0;
		virtual bool IsFullscreen() const = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual Ref<GraphicsContext> GetContext() const = 0;
		virtual Ref<Swapchain> GetSwapchain() const = 0;

		static Ref<Window> Create(const WindowCreateInfo& createInfo);
	};

}