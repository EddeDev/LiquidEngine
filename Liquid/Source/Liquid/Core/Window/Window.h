#pragma once

namespace Liquid {

	struct WindowCreateInfo
	{
		uint32 Width;
		uint32 Height;
		String Title;
	};

	class GraphicsContext;
	class Swapchain;

	class Window : public RefCounted
	{
	public:
		virtual ~Window() {}

		virtual void PollEvents() const = 0;
		virtual void SwapBuffers() const = 0;

		virtual Ref<GraphicsContext> GetContext() const = 0;
		virtual Ref<Swapchain> GetSwapchain() const = 0;

		static Ref<Window> Create(const WindowCreateInfo& createInfo);
	};

}