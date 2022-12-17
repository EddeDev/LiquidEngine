#pragma once

#include "Engine.h"

#include "Liquid/Renderer/API/GraphicsPipeline.h"
#include "Liquid/Renderer/API/Buffer.h"

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

		Ref<Buffer> m_VertexBuffer;
		Ref<Buffer> m_IndexBuffer;
	};

}