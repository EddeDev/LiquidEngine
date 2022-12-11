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

		static Ref<GraphicsContext> Create(const GraphicsContextCreateInfo& createInfo);
	};

}