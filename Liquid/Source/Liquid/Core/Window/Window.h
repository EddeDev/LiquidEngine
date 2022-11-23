#pragma once

namespace Liquid {

	struct WindowCreateInfo
	{
		uint32 Width = 1280;
		uint32 Height = 720;
		String Title = "Liquid Engine";
		bool Fullscreen = false;
		bool Decorated = true;
	};

	class Window : public RefCounted
	{
	public:
		virtual ~Window() {}

		virtual void PollEvents() const = 0;

		virtual void* GetHandle() const = 0;
		virtual void* GetPlatformHandle() const = 0;

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

		static Ref<Window> Create(const WindowCreateInfo& createInfo);
	};

}