#pragma once

#include "Engine.h"

namespace Liquid {

	class EditorEngine : public Engine
	{
	public:
		EditorEngine(const EngineCreateInfo& createInfo);

		virtual void OnInit() override;
		virtual void OnShutdown() override;
		virtual void OnUpdate() override;
		virtual void OnUIRender() override;
	private:
		Unique<ThemeBuilder> m_ThemeBuilder;
	};

}