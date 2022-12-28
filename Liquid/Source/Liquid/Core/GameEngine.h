#pragma once

#include "Engine.h"

#include "Liquid/Renderer/API/GraphicsPipeline.h"

#include "Liquid/Renderer/API/VertexBuffer.h"
#include "Liquid/Renderer/API/IndexBuffer.h"

namespace Liquid {

	class GameEngine : public Engine
	{
	public:
		GameEngine(const EngineCreateInfo& createInfo);

		virtual void OnInit() override;
		virtual void OnShutdown() override;
		virtual void OnUpdate() override;
	private:
		Ref<GraphicsPipeline> m_Pipeline;
		Ref<Shader> m_Shader;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}