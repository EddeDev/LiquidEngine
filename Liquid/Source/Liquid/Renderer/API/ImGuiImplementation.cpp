#include "LiquidPCH.h"
#include "ImGuiImplementation.h"

#include "Liquid/Core/Engine.h"

#include "DX11/DX11ImGuiImplementation.h"

namespace Liquid {

	Ref<ImGuiImplementation> ImGuiImplementation::Create(const ImGuiImplementationCreateInfo& createInfo)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11ImGuiImplementation>::Create(createInfo);
		}
		LQ_VERIFY(false, "Unknown Graphics API");
		return nullptr;
	}

}