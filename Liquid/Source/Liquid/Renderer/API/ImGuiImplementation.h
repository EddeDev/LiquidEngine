#pragma once

#include "Liquid/Renderer/RenderResource.h"

#include "Liquid/Core/Window/Window.h"
#include "Image.h"

#include <imgui.h>

namespace Liquid {

	struct ImGuiImplementationCreateInfo
	{
		Ref<Window> Window;
		ImGuiContext* Context;
	};

	class ImGuiImplementation : public RenderResource
	{
	public:
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void Image(Ref<Image2D> image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1)) const = 0;

		RENDER_RESOURCE_TYPE(ImGuiImplementation);

		static Ref<ImGuiImplementation> Create(const ImGuiImplementationCreateInfo& createInfo);
	};

}