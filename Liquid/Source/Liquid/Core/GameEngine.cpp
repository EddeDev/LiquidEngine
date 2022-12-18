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

		m_VertexBuffer = Buffer::Create(quadVertices, sizeof(QuadVertex) * 4, BUFFER_USAGE_VERTEX_BUFFER | BUFFER_USAGE_STATIC);
		m_IndexBuffer = Buffer::Create(quadIndices, sizeof(QuadIndex) * 2, BUFFER_USAGE_INDEX_BUFFER | BUFFER_USAGE_STATIC);
	
		GraphicsPipelineCreateInfo pipelineCreateInfo;
		pipelineCreateInfo.InputLayout = {
			{ "Position", ShaderDataType::Vec3 }
		};
		pipelineCreateInfo.Shader = Shader::Create("Resources/Shaders/VertexShader.hlsl", "Resources/Shaders/PixelShader.hlsl");

		m_Pipeline = GraphicsPipeline::Create(pipelineCreateInfo);
	}

	void GameEngine::OnShutdown()
	{

	}

	void GameEngine::OnUpdate()
	{
		m_Pipeline->DrawIndexed(m_VertexBuffer, m_IndexBuffer);
	}

}