#pragma once

#include "Liquid/Renderer/API/GraphicsPipeline.h"

namespace Liquid {

	class DX11Pipeline : public GraphicsPipeline
	{
	public:
		DX11Pipeline(const GraphicsPipelineCreateInfo& createInfo);
		virtual ~DX11Pipeline();
	private:
		GraphicsPipelineCreateInfo m_CreateInfo;
	};

}