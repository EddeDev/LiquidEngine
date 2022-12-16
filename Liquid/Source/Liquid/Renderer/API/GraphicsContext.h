#pragma once

#include "Liquid/Renderer/RenderResource.h"

namespace Liquid {

	struct GraphicsContextCreateInfo
	{
		void* WindowHandle = nullptr;

		bool EnableDebugLayers = false;
	};

	class GraphicsContext : public RenderResource
	{
	public:
		virtual ~GraphicsContext() {}

		RENDER_RESOURCE_TYPE(GraphicsContext);

		static Ref<GraphicsContext> Create(const GraphicsContextCreateInfo& createInfo);
	};

}