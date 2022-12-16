#include "LiquidPCH.h"
#include "GraphicsContext.h"

#include "Liquid/Core/Engine.h"

#include "DX11/DX11Context.h"

namespace Liquid {

	Ref<GraphicsContext> GraphicsContext::Create(const GraphicsContextCreateInfo& createInfo)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11Context>::Create(createInfo);
		}
		LQ_VERIFY(false, "Unknown Graphics API");
		return nullptr;
	}

}