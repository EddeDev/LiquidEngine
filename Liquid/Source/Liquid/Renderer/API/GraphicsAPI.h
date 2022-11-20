#pragma once

#include "Liquid/Core/PrimitiveTypes.h"

namespace Liquid {

	enum class GraphicsAPI : uint8
	{
		None = 0,
		DX11
	};

	namespace GraphicsAPIUtils {

		static const char* GetGraphicsAPIName(GraphicsAPI api, bool shortened = true)
		{
			switch (api)
			{
			case GraphicsAPI::DX11: return shortened ? "DX11" : "DirectX 11";
			}
			return "UnknownGraphicsAPI";
		}

	}

}