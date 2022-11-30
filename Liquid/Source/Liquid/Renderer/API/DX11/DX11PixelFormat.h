#pragma once

#include "Liquid/Core/Logging/LogMacros.h"
#include "Liquid/Core/AssertionMacros.h"

#include "Liquid/Renderer/PixelFormat.h"

#include "DX11Common.h"

namespace Liquid {

	static DXGI_FORMAT DX11PixelFormat(PixelFormat format)
	{
		switch (format)
		{
		case PixelFormat::RGBA:                   return DXGI_FORMAT_R8G8B8A8_UNORM;
		case PixelFormat::RGBA16F:                return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case PixelFormat::RGBA32F:                return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case PixelFormat::DEPTH24_STENCIL8:       return DXGI_FORMAT_R24G8_TYPELESS;
		case PixelFormat::DEPTH32F_STENCIL8_UINT: return DXGI_FORMAT_R32G8X24_TYPELESS;
		case PixelFormat::DEPTH32F:               return DXGI_FORMAT_R32_TYPELESS;
		}
		LQ_VERIFY(false, "Unknown image format");
		return DXGI_FORMAT_UNKNOWN;
	}

	static DXGI_FORMAT DX11SRVFormat(PixelFormat format)
	{
		switch (format)
		{
		case PixelFormat::RGBA:                   return DXGI_FORMAT_R8G8B8A8_UNORM;
		case PixelFormat::RGBA16F:                return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case PixelFormat::RGBA32F:                return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case PixelFormat::DEPTH24_STENCIL8:       return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case PixelFormat::DEPTH32F_STENCIL8_UINT: return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		case PixelFormat::DEPTH32F:               return DXGI_FORMAT_R32_FLOAT;
		}
		LQ_VERIFY(false, "Unknown image format");
		return DXGI_FORMAT_UNKNOWN;
	}

	static DXGI_FORMAT DX11DSVFormat(PixelFormat format)
	{
		switch (format)
		{
		case PixelFormat::DEPTH24_STENCIL8:       return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case PixelFormat::DEPTH32F_STENCIL8_UINT: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		case PixelFormat::DEPTH32F:               return DXGI_FORMAT_D32_FLOAT;
		}
		LQ_VERIFY(false, "Unknown image format");
		return DXGI_FORMAT_UNKNOWN;
	}

}