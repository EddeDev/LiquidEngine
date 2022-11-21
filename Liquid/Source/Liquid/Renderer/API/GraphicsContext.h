#pragma once

#include "GraphicsDevice.h"

namespace Liquid {

	struct GraphicsContextCreateInfo
	{
		void* WindowHandle = nullptr;

		bool EnableDebugLayers = false;
	};

	class GraphicsContext : public RefCounted
	{
	public:
		virtual ~GraphicsContext() {}

		virtual Ref<GraphicsDevice> GetDevice() const = 0;

		static Ref<GraphicsContext> Create(const GraphicsContextCreateInfo& createInfo);
	};

}