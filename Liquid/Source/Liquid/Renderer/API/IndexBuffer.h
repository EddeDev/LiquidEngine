#pragma once

#include "Liquid/Renderer/RenderResource.h"

namespace Liquid {

	enum class IndexBufferDataType
	{
		UnsignedInt = 0,
		UnsignedShort,
		UnsignedByte
	};

	class IndexBuffer : public RenderResource
	{
	public:
		RENDER_RESOURCE_TYPE(IndexBuffer);

		virtual void Bind() const = 0;
		virtual void RT_Bind() const = 0;

		virtual uint32 GetSize() const = 0;
		virtual uint32 GetCount() const = 0;

		virtual IndexBufferDataType GetDataType() const = 0;

		static Ref<IndexBuffer> Create(const void* data, uint32 size, IndexBufferDataType dataType = IndexBufferDataType::UnsignedInt);
		static Ref<IndexBuffer> Create(uint32 size, IndexBufferDataType dataType = IndexBufferDataType::UnsignedInt);
	};

	namespace IndexBufferUtils {

		static uint32 IndexBufferDataTypeSize(IndexBufferDataType type)
		{
			switch (type)
			{
			case IndexBufferDataType::UnsignedInt:   return sizeof(uint32);
			case IndexBufferDataType::UnsignedShort: return sizeof(uint16);
			case IndexBufferDataType::UnsignedByte:  return sizeof(uint8);
			}
			LQ_VERIFY(false, "Unknown IndexBufferDataType");
			return 0;
		}

	}

}