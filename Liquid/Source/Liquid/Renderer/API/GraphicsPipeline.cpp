#include "LiquidPCH.h"
#include "GraphicsPipeline.h"

#include "Liquid/Core/Engine.h"

#include "DX11/DX11Pipeline.h"

namespace Liquid {

	Ref<GraphicsPipeline> GraphicsPipeline::Create(const GraphicsPipelineCreateInfo& createInfo)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11Pipeline>::Create(createInfo);
		}
		LQ_VERIFY(false, "Unknown Graphics API");
		return nullptr;
	}

}