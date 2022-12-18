#include "LiquidPCH.h"
#include "Shader.h"

#include "Liquid/Core/Engine.h"

#include "DX11/DX11Shader.h"

namespace Liquid {

	Ref<Shader> Shader::Create(const String& vertexShaderPath, const String& pixelShaderPath)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11Shader>::Create(vertexShaderPath, pixelShaderPath);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

}