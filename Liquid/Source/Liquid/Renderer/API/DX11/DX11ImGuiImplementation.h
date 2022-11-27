#pragma once

#include "Liquid/Renderer/API/ImGuiImplementation.h"

namespace Liquid {

	class DX11ImGuiImplementation : public ImGuiImplementation
	{
	public:
		DX11ImGuiImplementation(const ImGuiImplementationCreateInfo& createInfo);
		virtual ~DX11ImGuiImplementation();

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void Image(Ref<Texture> texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1)) const override;
	};

}