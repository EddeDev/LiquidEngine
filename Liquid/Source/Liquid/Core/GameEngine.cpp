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
		};

		struct QuadIndex
		{
			uint32 V1;
			uint32 V2;
			uint32 V3;
		};

		QuadVertex quadVertices[4];
		quadVertices[0] = { { -0.5f, -0.5f, 0.0f } };
		quadVertices[1] = { {  0.5f, -0.5f, 0.0f } };
		quadVertices[2] = { {  0.5f,  0.5f, 0.0f } };
		quadVertices[3] = { { -0.5f,  0.5f, 0.0f } };

		QuadIndex quadIndices[2];
		quadIndices[0] = { 0, 1, 2 };
		quadIndices[1] = { 2, 3, 0 };

		m_VertexBuffer = VertexBuffer::Create(quadVertices, sizeof(QuadVertex) * 4);
		m_IndexBuffer = IndexBuffer::Create(quadIndices, sizeof(QuadIndex) * 2);

		m_Shader = Shader::Create("Resources/Shaders/VertexShader.hlsl", "Resources/Shaders/PixelShader.hlsl");

		GraphicsPipelineCreateInfo pipelineCreateInfo;
		pipelineCreateInfo.VertexElements = {
			{ "Position", VertexElementType::Float3 }
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
		m_Pipeline->DrawIndexed(m_IndexBuffer, 0, 4, 0, 2);
	}

}