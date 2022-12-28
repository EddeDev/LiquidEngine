#pragma once

#include "Liquid/Renderer/RenderResource.h"

namespace Liquid {

	enum class VertexBufferUsage
	{
		None = 0,
		Static,
		Dynamic
	};

	class VertexBuffer : public RenderResource
	{
	public:
		RENDER_RESOURCE_TYPE(VertexBuffer);

		virtual void Bind() const = 0;
		virtual void RT_Bind() const = 0;

		virtual uint32 GetSize() const = 0;

		static Ref<VertexBuffer> Create(const void* data, uint32 size, VertexBufferUsage usage = VertexBufferUsage::Static);
		static Ref<VertexBuffer> Create(uint32 size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
	};

}