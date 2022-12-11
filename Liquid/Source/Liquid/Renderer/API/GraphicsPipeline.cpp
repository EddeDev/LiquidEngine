#include "LiquidPCH.h"
#include "GraphicsPipeline.h"

#include "Liquid/Core/Application.h"

#include "DX11/DX11Pipeline.h"

namespace Liquid {

	Ref<GraphicsPipeline> GraphicsPipeline::Create(const GraphicsPipelineCreateInfo& createInfo)
	{
		switch (Application::GetGraphicsAPI())
		{
		case GraphicsAPI::DX11: return Ref<DX11Pipeline>::Create(createInfo);
		}
		LQ_VERIFY(false, "Unknown Graphics API");
		return nullptr;
	}

}