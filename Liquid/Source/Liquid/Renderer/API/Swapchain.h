#pragma once

#include "Liquid/Renderer/PixelFormat.h"

namespace Liquid {

	struct SwapchainCreateInfo
	{
		void* WindowHandle = nullptr;

		uint32 InitialWidth = 0;
		uint32 InitialHeight = 0;

		bool InitialFullscreenState = false;
		bool AllowTearing = true;

		PixelFormat ColorFormat = PixelFormat::RGBA;
		PixelFormat DepthFormat = PixelFormat::DEPTH24_STENCIL8;

		uint32 BufferCount = 3;
		uint32 SampleCount = 1;
	};

	enum SwapchainBufferType
	{
		BUFFER_NONE = 0,
		BUFFER_COLOR = BIT(0),
		BUFFER_DEPTH = BIT(1),
		BUFFER_STENCIL = BIT(2)
	};

	class Swapchain : public RefCounted
	{
	public:
		virtual ~Swapchain() {}

		virtual void Resize(uint32 width, uint32 height, bool fullscreen) = 0;

		virtual void BeginFrame() const = 0;
		virtual void Present() const = 0;
		virtual void Clear(uint32 buffer) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSyncEnabled() const = 0;

		static Ref<Swapchain> Create(const SwapchainCreateInfo& createInfo);
	};

}