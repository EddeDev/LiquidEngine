#include "LiquidPCH.h"
#include "Image.h"

#include "Liquid/Core/Engine.h"

#include "DX11/DX11Image.h"

namespace Liquid {

	Ref<Image2D> Image2D::Create(const ImageCreateInfo& createInfo)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11Image2D>::Create(createInfo);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

}