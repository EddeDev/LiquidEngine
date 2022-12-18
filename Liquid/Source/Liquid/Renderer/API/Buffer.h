#pragma once

#include "Liquid/Renderer/RenderResource.h"

namespace Liquid {

	enum class BufferUsage : uint32
	{
		None = 0,
		
		Static = BIT(0),
		Dynamic = BIT(1),
		Volatile = BIT(2),

		VertexBuffer = BIT(3),
		IndexBuffer = BIT(4),

		ShaderResource = BIT(5),
		UnorderedAccess = BIT(6),
		DrawIndirect = BIT(7),
		Shared = BIT(8)
	};
	DEFINE_ENUM_OPERATORS(BufferUsage);

#define BUFFER_USAGE_NONE BufferUsage::None
#define BUFFER_USAGE_STATIC BufferUsage::Static
#define BUFFER_USAGE_DYNAMIC BufferUsage::Dynamic
#define BUFFER_USAGE_VERTEX_BUFFER BufferUsage::VertexBuffer
#define BUFFER_USAGE_INDEX_BUFFER BufferUsage::IndexBuffer
#define BUFFER_USAGE_SHADER_RESOURCE BufferUsage::ShaderResource
#define BUFFER_USAGE_UNORDERED_ACCESS BufferUsage::UnorderedAccess
#define BUFFER_USAGE_DRAW_INDIRECT BufferUsage::DrawIndirect
#define BUFFER_USAGE_SHARED BufferUsage::Shared

	class Buffer : public RenderResource
	{
	public:
		RENDER_RESOURCE_TYPE(Buffer);

		virtual uint32 GetSize() const = 0;

		static Ref<Buffer> Create(const void* data, uint32 size, BufferUsage usage);
		static Ref<Buffer> Create(uint32 size, BufferUsage usage);
	};

}