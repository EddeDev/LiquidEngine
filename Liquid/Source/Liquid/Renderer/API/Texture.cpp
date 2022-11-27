#include "LiquidPCH.h"
#include "Texture.h"

#include "Liquid/Core/Application.h"

#include "DX11/DX11Texture.h"

namespace Liquid {

	Ref<Texture> Texture::Create(const String& filepath)
	{
		switch (Application::GetGraphicsAPI())
		{
		case GraphicsAPI::DX11: return Ref<DX11Texture>::Create(filepath);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

}