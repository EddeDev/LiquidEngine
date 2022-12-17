#include "LiquidPCH.h"
#include "Buffer.h"

#include "Liquid/Core/Engine.h"

#include "DX11/DX11Buffer.h"

namespace Liquid {

	Ref<Buffer> Buffer::Create(const void* data, uint32 size, BufferUsage usage)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11Buffer>::Create(data, size, usage);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ref<Buffer> Buffer::Create(uint32 size, BufferUsage usage)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11Buffer>::Create(size, usage);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

}