#include "LiquidPCH.h"
#include "GraphicsContext.h"

#include "Liquid/Core/Application.h"

#include "DX11/DX11Context.h"

namespace Liquid {

	Ref<GraphicsContext> GraphicsContext::Create(const GraphicsContextCreateInfo& createInfo)
	{
		switch (Application::GetGraphicsAPI())
		{
		case GraphicsAPI::DX11: s_Instance = Ref<DX11Context>::Create(createInfo); break;
		}
		return s_Instance;
	}

}