#pragma once

#include "Liquid/Renderer/API/GraphicsPipeline.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11Pipeline : public GraphicsPipeline
	{
	public:
		DX11Pipeline(const GraphicsPipelineCreateInfo& createInfo);
		virtual ~DX11Pipeline();

		virtual void DrawIndexed(Ref<Buffer> vertexBuffer, Ref<Buffer> indexBuffer) const override;
	private:
		GraphicsPipelineCreateInfo m_CreateInfo;
	};

}