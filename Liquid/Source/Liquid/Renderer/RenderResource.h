#pragma once

#include "Liquid/Core/RefCounting.h"

#include <algorithm>

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
	protected:
		RenderResource();
	public:
		virtual ~RenderResource() {}

		virtual RenderResourceType GetType() const = 0;

		time_t GetTime() const { return m_Time; }
	private:
		time_t m_Time = 0;
	};

	using RenderResourceList = std::vector<RenderResource*>;
	using RenderResourceMap = std::unordered_map<RenderResourceType, RenderResourceList>;

	class RenderResourceManager
	{
	public:
		static uint32 GetResourceCount(RenderResourceType type);

		static const RenderResourceMap& GetGlobalReferences() { return s_GlobalReferences; }
	private:
		static bool HasResource(RenderResource* resource);
		static void OnResourceAdded(RenderResource* resource);
	private:
		static RenderResourceMap s_GlobalReferences;

		friend class RenderResource;
	};

	namespace RenderResourceUtils {

		static const char* RenderResourceTypeToString(RenderResourceType type)
		{
			switch (type)
			{
			case RenderResourceType::GraphicsContext: return "GraphicsContext";
			case RenderResourceType::GraphicsDevice: return "GraphicsDevice";
			case RenderResourceType::GraphicsPipeline: return "GraphicsPipeline";
			case RenderResourceType::Swapchain: return "Swapchain";
			case RenderResourceType::ImGuiImplementation: return "ImGuiImplementation";
			case RenderResourceType::Shader: return "Shader";
			case RenderResourceType::Image2D: return "Image2D";
			}
			return "UnknownRenderResourceType";
		}

	}

}