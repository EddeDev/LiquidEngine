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

		static uint32 ComputeBytesPerPixel(PixelFormat format)
		{
			uint32 bytesPerPixel = 0;

			switch (format)
			{
			case PixelFormat::RGBA:
				bytesPerPixel = 4;
				break;
			case PixelFormat::RGBA16F:
				bytesPerPixel = 8;
				break;
			case PixelFormat::RGBA32F:
				bytesPerPixel = 16;
				break;
			}

			LQ_ASSERT(bytesPerPixel, "Unknown pixel format");
			return bytesPerPixel;
		}

	}

}