#pragma once

#include "Liquid/Renderer/RenderResource.h"

#include "Shader.h"
#include "Buffer.h"

namespace Liquid {

	enum class PrimitiveTopology : uint8
	{
		Triangles = 0
	};

	enum class ShaderDataType : uint8
	{
		None = 0,
		Float, Vec2, Vec3, Vec4,
		Mat3, Mat4,
		Int, IVec2, IVec3, IVec4,
		Bool
	};

	static uint32 ShaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 1;
		case ShaderDataType::Vec2:  return 2;
		case ShaderDataType::Vec3:  return 3;
		case ShaderDataType::Vec4:  return 4;
		case ShaderDataType::Mat3:  return 3 * 3;
		case ShaderDataType::Mat4:  return 4 * 4;
		case ShaderDataType::Int:   return 1;
		case ShaderDataType::IVec2: return 2;
		case ShaderDataType::IVec3: return 3;
		case ShaderDataType::IVec4: return 4;
		case ShaderDataType::Bool:  return 1;
		}
		LQ_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	static uint32 ShaderDataTypeSize(ShaderDataType type)
	{
		const uint32 componentCount = ShaderDataTypeComponentCount(type);
		switch (type)
		{
		case ShaderDataType::Float: return sizeof(float) * componentCount;
		case ShaderDataType::Vec2:  return sizeof(float) * componentCount;
		case ShaderDataType::Vec3:  return sizeof(float) * componentCount;
		case ShaderDataType::Vec4:  return sizeof(float) * componentCount;
		case ShaderDataType::Mat3:  return sizeof(float) * componentCount;
		case ShaderDataType::Mat4:  return sizeof(float) * componentCount;
		case ShaderDataType::Int:   return sizeof(int32) * componentCount;
		case ShaderDataType::IVec2: return sizeof(int32) * componentCount;
		case ShaderDataType::IVec3: return sizeof(int32) * componentCount;
		case ShaderDataType::IVec4: return sizeof(int32) * componentCount;
		case ShaderDataType::Bool:  return sizeof(bool)  * componentCount;
		}
		LQ_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct VertexInputAttribute
	{
		std::string Name;
		ShaderDataType Type = ShaderDataType::None;
		uint32 Size = 0;
		uint32 ComponentCount = 0;
		uint32 Offset = 0;
		bool Normalized = false;

		VertexInputAttribute() = default;
		VertexInputAttribute(const std::string& name, ShaderDataType type, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), ComponentCount(ShaderDataTypeComponentCount(type)), Normalized(normalized)
		{
		}
	};

	class VertexInputLayout
	{
	public:
		VertexInputLayout() = default;
		VertexInputLayout(const std::initializer_list<VertexInputAttribute>& attributes)
			: m_Attributes(attributes)
		{
			uint32 offset = m_Offset;
			for (auto& attribute : m_Attributes)
			{
				attribute.Offset = offset;
				offset += attribute.Size;
				m_Stride += attribute.Size;
			}
		}

		uint32 GetOffset() const { return m_Offset; };
		uint32 GetStride() const { return m_Stride; };
		uint32 GetAttributeCount() const { return (uint32)m_Attributes.size(); };
		const std::vector<VertexInputAttribute>& GetAttributes() const { return m_Attributes; }

		[[nodiscard]] std::vector<VertexInputAttribute>::iterator begin() { return m_Attributes.begin(); }
		[[nodiscard]] std::vector<VertexInputAttribute>::iterator end() { return m_Attributes.end(); }
		[[nodiscard]] std::vector<VertexInputAttribute>::const_iterator begin() const { return m_Attributes.begin(); }
		[[nodiscard]] std::vector<VertexInputAttribute>::const_iterator end() const { return m_Attributes.end(); }
	private:
		std::vector<VertexInputAttribute> m_Attributes;
		uint32 m_Offset = 0;
		uint32 m_Stride = 0;
	};

	struct GraphicsPipelineCreateInfo
	{
		VertexInputLayout InputLayout;
		Ref<Shader> Shader;

		PrimitiveTopology Topology = PrimitiveTopology::Triangles;
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

		virtual void DrawIndexed(Ref<Buffer> vertexBuffer, Ref<Buffer> indexBuffer) const = 0;

		RENDER_RESOURCE_TYPE(GraphicsPipeline);

		static Ref<GraphicsPipeline> Create(const GraphicsPipelineCreateInfo& createInfo);
	};

}