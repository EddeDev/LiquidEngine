#include "LiquidPCH.h"
#include "GameEngine.h"

#include <glm/glm.hpp>

namespace Liquid {

	GameEngine::GameEngine(const EngineCreateInfo& createInfo)
		: Engine(createInfo)
	{
	}

	void GameEngine::OnInit()
	{
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		struct QuadIndex
		{
			uint32 V1;
			uint32 V2;
			uint32 V3;
		};

		float size = 1.0f;

		QuadVertex quadVertices[4];
		quadVertices[0] = { { -size, -size, 0.0f }, { 0.8f, 0.8f, 0.8f, 1.0f } };
		quadVertices[1] = { {  size, -size, 0.0f }, { 0.8f, 0.8f, 0.8f, 1.0f } };
		quadVertices[2] = { {  size,  size, 0.0f }, { 0.2f, 0.4f, 0.8f, 1.0f } };
		quadVertices[3] = { { -size,  size, 0.0f }, { 0.2f, 0.4f, 0.8f, 1.0f } };

		QuadIndex quadIndices[2];
		quadIndices[0] = { 0, 1, 2 };
		quadIndices[1] = { 2, 3, 0 };

		m_VertexBuffer = VertexBuffer::Create(quadVertices, sizeof(QuadVertex) * 4);
		m_IndexBuffer = IndexBuffer::Create(quadIndices, sizeof(QuadIndex) * 2);

		m_Shader = Shader::Create("Resources/Shaders/VertexShader.hlsl", "Resources/Shaders/PixelShader.hlsl");

		GraphicsPipelineCreateInfo pipelineCreateInfo;
		pipelineCreateInfo.VertexElements = {
			{ "Position", VertexElementType::Float3 },
			{ "Color", VertexElementType::Float4 }
		};
		pipelineCreateInfo.Shader = m_Shader;
		m_Pipeline = GraphicsPipeline::Create(pipelineCreateInfo);
	}

	void GameEngine::OnShutdown()
	{

	}

	void GameEngine::OnUpdate()
	{
		m_VertexBuffer->Bind();
		m_Pipeline->Bind();
		m_IndexBuffer->Bind();

		m_Shader->Bind();
		m_Pipeline->DrawIndexed(m_IndexBuffer->GetCount());
	}

}