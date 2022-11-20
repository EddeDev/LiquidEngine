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

		template<typename TDerived>
		inline static Ref<TDerived> Get()
		{
			return s_Instance.As<TDerived>();
		}

		static Ref<GraphicsContext> Create(const GraphicsContextCreateInfo& createInfo);
	private:
		inline static Ref<GraphicsContext> s_Instance;
	};

}