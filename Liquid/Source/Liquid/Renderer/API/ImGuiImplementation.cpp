#include "LiquidPCH.h"
#include "ImGuiImplementation.h"

#include "Liquid/Core/Application.h"

#include "DX11/DX11ImGuiImplementation.h"

namespace Liquid {

	Ref<ImGuiImplementation> ImGuiImplementation::Create()
	{
		switch (Application::GetGraphicsAPI())
		{
		case GraphicsAPI::DX11: return Ref<DX11ImGuiImplementation>::Create();
		}
		LQ_VERIFY(false, "Unknown Graphics API");
		return nullptr;
	}

}