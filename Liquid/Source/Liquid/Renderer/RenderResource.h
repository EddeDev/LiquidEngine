#pragma once

#include "Liquid/Core/RefCounting.h"

namespace Liquid {

	enum class RenderResourceType : uint8
	{
		None = 0,

		GraphicsContext,
		GraphicsDevice,
		GraphicsPipeline,
		Swapchain,
		ImGuiImplementation,
		Shader,
		Image2D
	};

#define RENDER_RESOURCE_TYPE(type) \
	static RenderResourceType GetStaticType() \
	{ \
		return RenderResourceType::##type; \
	} \
	virtual RenderResourceType GetType() const override \
	{ \
		return GetStaticType(); \
	}

	class RenderResource : public RefCounted
	{
	public:
		virtual ~RenderResource() {}

		virtual RenderResourceType GetType() const = 0;
	};

}