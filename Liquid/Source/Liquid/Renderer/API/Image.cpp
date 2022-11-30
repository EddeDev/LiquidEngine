#include "LiquidPCH.h"
#include "Image.h"

#include "Liquid/Core/Application.h"

#include "DX11/DX11Image.h"

namespace Liquid {

	Ref<Image2D> Image2D::Create(const ImageCreateInfo& createInfo)
	{
		switch (Application::GetGraphicsAPI())
		{
		case GraphicsAPI::DX11: return Ref<DX11Image2D>::Create(createInfo);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

}