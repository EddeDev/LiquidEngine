#pragma once

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

		static Ref<GraphicsContext> Create(const GraphicsContextCreateInfo& createInfo);
	};

}