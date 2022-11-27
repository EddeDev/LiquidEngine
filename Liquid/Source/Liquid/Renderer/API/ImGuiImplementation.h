#pragma once

#include "Texture.h"

#include <imgui.h>

namespace Liquid {

	struct ImGuiImplementationCreateInfo
	{
		void* WindowHandle;
	};

	class ImGuiImplementation : public RefCounted
	{
	public:
		virtual ~ImGuiImplementation() {}

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void Image(Ref<Texture> texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1)) const = 0;

		static Ref<ImGuiImplementation> Create(const ImGuiImplementationCreateInfo& createInfo);
	};

}