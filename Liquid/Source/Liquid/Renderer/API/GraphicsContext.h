#pragma once

#include "Liquid/Core/Application.h"

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

		template<typename T>
		static Ref<T> Get()
		{
			return Application::GetContext().As<T>();
		}

		static Ref<GraphicsContext> Create(const GraphicsContextCreateInfo& createInfo);
	};

}