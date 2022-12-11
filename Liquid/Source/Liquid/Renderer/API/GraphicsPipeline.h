#pragma once

#include "Liquid/Renderer/RenderResource.h"

#include "Shader.h"

namespace Liquid {

	struct GraphicsPipelineCreateInfo
	{
		Ref<Shader> Shader;
	};

	class GraphicsPipeline : public RenderResource
	{
	public:
		virtual ~GraphicsPipeline() {}

		static Ref<GraphicsPipeline> Create(const GraphicsPipelineCreateInfo& createInfo);
	};

}