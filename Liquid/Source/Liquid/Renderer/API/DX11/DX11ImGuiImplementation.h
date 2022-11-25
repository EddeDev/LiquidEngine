#pragma once

#include "Liquid/Renderer/API/ImGuiImplementation.h"

namespace Liquid {

	class DX11ImGuiImplementation : public ImGuiImplementation
	{
	public:
		DX11ImGuiImplementation();
		virtual ~DX11ImGuiImplementation();

		virtual void BeginFrame() override;
		virtual void EndFrame() override;
	};

}