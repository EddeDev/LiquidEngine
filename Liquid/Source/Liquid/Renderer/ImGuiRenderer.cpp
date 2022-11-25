#include "LiquidPCH.h"
#include "ImGuiRenderer.h"

#include "API/ImGuiImplementation.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Liquid {

	static Ref<ImGuiImplementation> s_Implementation;

	void ImGuiRenderer::Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf", 18.0f);

		// ImGui::StyleColorsDark();

		InitDefaultStyle();

		s_Implementation = ImGuiImplementation::Create();
	}

	void ImGuiRenderer::Shutdown()
	{
		if (s_Implementation->GetReferenceCount() != 1)
			LQ_PLATFORM_BREAK();

		s_Implementation = nullptr;

		ImGui::DestroyContext();
	}

	void ImGuiRenderer::BeginFrame()
	{
		s_Implementation->BeginFrame();

		ImGui::NewFrame();
	}

	void ImGuiRenderer::EndFrame()
	{
		ImGui::Render();

		s_Implementation->EndFrame();

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiRenderer::InitDefaultStyle()
	{
		// Style
		auto& style = ImGui::GetStyle();
		style.Alpha = 1.0f;
		style.DisabledAlpha = 0.6f;
		style.WindowPadding = { 8.0f, 8.0f };
		style.WindowRounding = 1.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = { 32.0f, 32.0f };
		style.WindowTitleAlign = { 0.02f, 0.5f };
		style.ChildRounding = 1.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = { 4.0f, 3.0f };
		style.FrameRounding = 4.0f;
		style.FrameBorderSize = 1.0f;
		style.ItemSpacing = { 8.0f, 4.0f };
		style.ItemInnerSpacing = { 4.0f, 4.0f };
		style.IndentSpacing = 18.0f;
		style.CellPadding = { 4.0f, 2.0f };
		style.ScrollbarSize = 14.0f;
		style.ScrollbarRounding = 9.0f;
		style.GrabMinSize = 12.0f;
		style.GrabRounding = 2.0f;
		style.TabRounding = 5.0f;
		style.ButtonTextAlign = { 0.5f, 0.5f };
		style.SelectableTextAlign = { 0.0f, 0.0f };

		// Colors
		auto& colors = style.Colors;
		colors[ImGuiCol_Text]                  = ImVec4(0.88f, 0.88f, 0.88f, 1.0f);
		colors[ImGuiCol_TextDisabled]          = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		colors[ImGuiCol_WindowBg]              = ImVec4(0.0883f, 0.0902f, 0.0883f, 1.0f);
		colors[ImGuiCol_ChildBg]               = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border]                = ImVec4(0.1882f, 0.1863f, 0.1882f, 1.0f);
		colors[ImGuiCol_BorderShadow]          = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_FrameBg]               = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
		colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.1589f, 0.1608f, 0.1589f, 1.0f);
		colors[ImGuiCol_FrameBgActive]         = ImVec4(0.2392f, 0.2373f, 0.2392f, 1.0f);
		colors[ImGuiCol_TitleBg]               = ImVec4(0.0883f, 0.0902f, 0.0883f, 1.0f);
		colors[ImGuiCol_TitleBgActive]         = ImVec4(0.1608f, 0.1588f, 0.1608f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
		colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
		colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
		colors[ImGuiCol_CheckMark]             = ImVec4(0.25f, 0.58f, 0.7f, 1.0f);
		colors[ImGuiCol_SliderGrab]            = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);
		colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.2902f, 0.2882f, 0.2902f, 1.0f);
		colors[ImGuiCol_Button]                = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);
		colors[ImGuiCol_ButtonHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
		colors[ImGuiCol_ButtonActive]          = ImVec4(0.3098f, 0.3078f, 0.3098f, 1.0f);
		colors[ImGuiCol_Header]                = ImVec4(0.18f, 0.18f, 0.19f, 1.0f);
		colors[ImGuiCol_HeaderHovered]         = ImVec4(0.23f, 0.23f, 0.24f, 1.0f);
		colors[ImGuiCol_HeaderActive]          = ImVec4(0.31f, 0.3f, 0.31f, 1.0f);
		colors[ImGuiCol_Separator]             = ImVec4(0.4294f, 0.4294f, 0.4314f, 0.502f);
		colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.1f, 0.4f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive]       = ImVec4(0.1f, 0.4f, 0.75f, 1.0f);
		colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.2f);
		colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab]                   = ImVec4(0.149f, 0.1471f, 0.149f, 1.0f);
		colors[ImGuiCol_TabHovered]            = ImVec4(0.2f, 0.198f, 0.2f, 1.0f);
		colors[ImGuiCol_TabActive]             = ImVec4(0.2373f, 0.2392f, 0.2373f, 1.0f);
		colors[ImGuiCol_TabUnfocused]          = ImVec4(0.149f, 0.1471f, 0.149f, 1.0f);
		colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.1981f, 0.2f, 0.1981f, 1.0f);
		colors[ImGuiCol_DockingPreview]        = ImVec4(0.71f, 0.66f, 0.14f, 0.7f);
		colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
		colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.0f, 0.43f, 0.35f, 1.0f);
		colors[ImGuiCol_PlotHistogram]         = ImVec4(0.9f, 0.7f, 0.0f, 1.0f);
		colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
		colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.1863f, 0.1863f, 0.1882f, 1.0f);
		colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.3098f, 0.3098f, 0.3078f, 1.0f);
		colors[ImGuiCol_TableBorderLight]      = ImVec4(0.2314f, 0.2314f, 0.2294f, 1.0f);
		colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
		colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget]        = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
		colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
		colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
		colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
	}

}