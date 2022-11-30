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

		virtual void Image(Ref<Image2D> image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1)) const override;
	private:
		void InstallCallbacks();

		void OnWindowFocusCallback(int32 focused);
		void OnCursorEnterCallback(int32 entered);
		void OnCursorPosCallback(double x, double y);
		void OnMouseButtonCallback(int32 button, int32 action, int32 mods);
		void OnScrollCallback(double xoffset, double yoffset);
		void OnKeyCallback(int32 key, int32 scancode, int32 action, int32 mods);
		void OnCharCallback(uint32 codepoint);
	private:
		ImGuiImplementationCreateInfo m_CreateInfo;
	};

}