#pragma once

#include "Liquid/Renderer/RenderResource.h"

#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Liquid {

	enum class PrimitiveTopology : uint8
	{
		TriangleList = 0,
		TriangleStrip,
		LineList,
		PointList
	};

	enum class VertexElementType : uint8
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static uint32 VertexElementTypeComponentCount(VertexElementType type)
	{
		switch (type)
		{
		case VertexElementType::Float:  return 1;
		case VertexElementType::Float2: return 2;
		case VertexElementType::Float3: return 3;
		case VertexElementType::Float4: return 4;
		case VertexElementType::Int:    return 1;
		case VertexElementType::Int2:   return 2;
		case VertexElementType::Int3:   return 3;
		case VertexElementType::Int4:   return 4;
		case VertexElementType::Bool:   return 1;
		}
		LQ_VERIFY(false, "Unknown VertexElementType");
		return 0;
	}

	static uint32 VertexElementTypeSize(VertexElementType type)
	{
		const uint32 componentCount = VertexElementTypeComponentCount(type);
		switch (type)
		{
		case VertexElementType::Float:  return sizeof(float) * componentCount;
		case VertexElementType::Float2: return sizeof(float) * componentCount;
		case VertexElementType::Float3: return sizeof(float) * componentCount;
		case VertexElementType::Float4: return sizeof(float) * componentCount;
		case VertexElementType::Int:    return sizeof(int32) * componentCount;
		case VertexElementType::Int2:   return sizeof(int32) * componentCount;
		case VertexElementType::Int3:   return sizeof(int32) * componentCount;
		case VertexElementType::Int4:   return sizeof(int32) * componentCount;
		case VertexElementType::Bool:   return sizeof(bool) * componentCount;
		}
		LQ_VERIFY(false, "Unknown VertexElementType");
		return 0;
	}

	struct VertexElement
	{
		String Name;
		VertexElementType Type;
		uint32 Size;
		uint32 ComponentCount;
		uint32 Offset;

		VertexElement() = default;
		VertexElement(String name, VertexElementType type) :
			Name(name),
			Type(type),
			Size(VertexElementTypeSize(type)),
			ComponentCount(VertexElementTypeComponentCount(type)),
			Offset(0)
		{}
	};

	class VertexElementLayout
	{
	public:
		VertexElementLayout() = default;
		VertexElementLayout(const std::initializer_list<VertexElement>& elements)
			: m_Elements(elements)
		{
			uint32 offset = m_Offset;
			for (auto& attribute : m_Elements)
			{
				attribute.Offset = offset;
				offset += attribute.Size;
				m_Stride += attribute.Size;
			}
		}

		uint32 GetOffset() const { return m_Offset; };
		uint32 GetStride() const { return m_Stride; };
		uint32 GetCount() const { return (uint32)m_Elements.size(); };

		std::vector<VertexElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<VertexElement>::iterator end() { return m_Elements.end(); }
		std::vector<VertexElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<VertexElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		std::vector<VertexElement> m_Elements;
		uint32 m_Offset = 0;
		uint32 m_Stride = 0;
	};

	struct GraphicsPipelineCreateInfo
	{
		VertexElementLayout VertexElements;

		Ref<Shader> Shader;

		PrimitiveTopology Topology = PrimitiveTopology::TriangleList;
		bool DepthTest = true;
		bool DepthWrite = true;
		bool BackfaceCulling = false;
		bool Wireframe = false;
	};

	class GraphicsPipeline : public RenderResource
	{
	public:
		virtual ~GraphicsPipeline() {}

		virtual void Invalidate() = 0;
		virtual void RT_Invalidate() = 0;

		virtual void Bind() const = 0;
		virtual void RT_Bind() const = 0;

		virtual void DrawIndexed(uint32 indexCount, uint32 startIndexLocation = 0, uint32 baseVertexLocation = 0) const = 0;
		virtual void RT_DrawIndexed(uint32 indexCount, uint32 startIndexLocation = 0, uint32 baseVertexLocation = 0) const = 0;

		RENDER_RESOURCE_TYPE(GraphicsPipeline);

		static Ref<GraphicsPipeline> Create(const GraphicsPipelineCreateInfo& createInfo);
	};

}