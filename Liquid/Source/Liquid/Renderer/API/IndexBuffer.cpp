#include "LiquidPCH.h"
#include "IndexBuffer.h"

#include "Liquid/Core/Engine.h"

#include "DX11/DX11IndexBuffer.h"

namespace Liquid {

	Ref<IndexBuffer> IndexBuffer::Create(const void* data, uint32 size, IndexBufferDataType dataType)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11IndexBuffer>::Create(data, size, dataType);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32 size, IndexBufferDataType dataType)
	{
		switch (LQ_CURRENT_GRAPHICS_API)
		{
		case GraphicsAPI::DX11: return Ref<DX11IndexBuffer>::Create(size, dataType);
		}
		LQ_VERIFY(false, "Unknown GraphicsAPI");
		return nullptr;
	}

}