#pragma once

#include "Liquid/Renderer/API/ImGuiRenderer.h"

namespace Liquid {

	class DX11ImGuiRenderer : public ImGuiRenderer
	{
	public:
		DX11ImGuiRenderer() = default;
	protected:
		virtual void Init() override;
		virtual void NewFrame() override;
		virtual void Render() override;
	};

}