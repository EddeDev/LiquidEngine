#pragma once

#include "Liquid/Renderer/RenderResource.h"

#include "Shader.h"
#include "Buffer.h"

namespace Liquid {

	struct GraphicsPipelineCreateInfo
	{
		Ref<Shader> Shader;
	};

	class GraphicsPipeline : public RenderResource
	{
	public:
		virtual ~GraphicsPipeline() {}

		virtual void DrawIndexed(Ref<Buffer> vertexBuffer, Ref<Buffer> indexBuffer) const = 0;

		RENDER_RESOURCE_TYPE(GraphicsPipeline);

		static Ref<GraphicsPipeline> Create(const GraphicsPipelineCreateInfo& createInfo);
	};

}