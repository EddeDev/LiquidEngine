#pragma once

namespace Liquid {

	struct WindowCreateInfo
	{
		uint32 Width;
		uint32 Height;
		String Title;
	};

	class Window : public RefCounted
	{
	public:
		virtual ~Window() {}

		virtual void PollEvents() const = 0;

		static Ref<Window> Create(const WindowCreateInfo& createInfo);
	};

}