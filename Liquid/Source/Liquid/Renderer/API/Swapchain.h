#pragma once

#include "Liquid/Renderer/RenderResource.h"

#include "Liquid/Renderer/PixelFormat.h"

namespace Liquid {

	struct SwapchainCreateInfo
	{
		void* WindowHandle = nullptr;

		uint32 InitialWidth = 0;
		uint32 InitialHeight = 0;

		bool InitialFullscreenState = false;
		bool InitialVSyncState = true;
		bool AllowTearing = true;

		PixelFormat ColorFormat = PixelFormat::RGBA;
		PixelFormat DepthFormat = PixelFormat::DEPTH24_STENCIL8;

		uint32 BufferCount = 3;
		uint32 SampleCount = 1;
	};

	enum class SwapchainBufferType : uint32
	{
		None = 0,
		Color = BIT(0),
		Depth = BIT(1),
		Stencil = BIT(2)
	};
	DEFINE_ENUM_OPERATORS(SwapchainBufferType);

#define SB_COLOR SwapchainBufferType::Color
#define SB_DEPTH SwapchainBufferType::Depth
#define SB_STENCIL SwapchainBufferType::Stencil

	class Swapchain : public RenderResource
	{
	public:
		virtual void Resize(uint32 width, uint32 height, bool fullscreen) = 0;

		virtual void BeginFrame() const = 0;
		virtual void Present() const = 0;
		virtual void Clear(SwapchainBufferType buffer) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSyncEnabled() const = 0;

		RENDER_RESOURCE_TYPE(Swapchain);

		static Ref<Swapchain> Create(const SwapchainCreateInfo& createInfo);
	};

}