#pragma once

#include "Liquid/Core/PrimitiveTypes.h"

namespace Liquid {

	enum class PixelFormat : uint8
	{
		None = 0,

		// Color formats
		RGBA,
		RGBA16F,
		RGBA32F,

		// Depth formats
		DEPTH24_STENCIL8,
		DEPTH32F_STENCIL8_UINT,
		DEPTH32F
	};

	namespace PixelFormatUtils {

		static bool IsStencilFormat(PixelFormat format)
		{
			if (format == PixelFormat::DEPTH24_STENCIL8 || format == PixelFormat::DEPTH32F_STENCIL8_UINT)
				return true;

			return false;
		}

	}

}