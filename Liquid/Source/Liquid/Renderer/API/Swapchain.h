#pragma once

#include "GraphicsContext.h"

#include "Liquid/Renderer/PixelFormat.h"

namespace Liquid {

	struct SwapchainCreateInfo
	{
		Ref<GraphicsContext> Context;
		void* WindowHandle;

		uint32 InitialWidth = 0;
		uint32 InitialHeight = 0;

		PixelFormat ColorFormat = PixelFormat::RGBA;
		PixelFormat DepthFormat = PixelFormat::DEPTH24_STENCIL8;

		uint32 BufferCount = 3;
		uint32 SampleCount = 1;
	};

	class Swapchain : public RefCounted
	{
	public:
		virtual ~Swapchain() {}

		virtual void Resize(uint32 width, uint32 height) = 0;
		virtual void Present() const = 0;

		static Ref<Swapchain> Create(const SwapchainCreateInfo& createInfo);
	};

}