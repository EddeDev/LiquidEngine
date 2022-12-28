#include "LiquidPCH.h"
#include "VertexBuffer.h"

#include "Liquid/Core/Engine.h"

#include "DX11/DX11VertexBuffer.h"

namespace Liquid {

	Ref<VertexBuffer> VertexBuffer::Create(const void* data, uint32 size, VertexBufferUsage usage)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11VertexBuffer>::Create(data, size, usage);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32 size, VertexBufferUsage usage)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11VertexBuffer>::Create(size, usage);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

}