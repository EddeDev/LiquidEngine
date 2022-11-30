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

		static bool IsDepthFormat(PixelFormat format)
		{
			return format == PixelFormat::DEPTH24_STENCIL8 || format == PixelFormat::DEPTH32F_STENCIL8_UINT || format == PixelFormat::DEPTH32F;
		}

		static bool IsStencilFormat(PixelFormat format)
		{
			return format == PixelFormat::DEPTH24_STENCIL8 || format == PixelFormat::DEPTH32F_STENCIL8_UINT;
		}

		static uint32 GetBPP(PixelFormat format)
		{
			switch (format)
			{
			case PixelFormat::RGBA:    return 4 * 1;
			case PixelFormat::RGBA32F: return 4 * 4;
			}
			LQ_VERIFY(false, "Unknown image format");
			return 0;
		}

	}

}