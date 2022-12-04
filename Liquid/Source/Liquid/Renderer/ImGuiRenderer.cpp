#include "LiquidPCH.h"
#include "ImGuiRenderer.h"

#include "API/ImGuiImplementation.h"

#include <imgui.h>

namespace Liquid {

	std::unordered_map<ImGuiContext*, ImGuiRenderer*> ImGuiRenderer::s_ContextMap;

	ImGuiRenderer::ImGuiRenderer(const ImGuiRendererCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		IMGUI_CHECKVERSION();

		m_PreviousContext = ImGui::GetCurrentContext();
		m_Context = IM_NEW(ImGuiContext)(nullptr);
		s_ContextMap[m_Context] = this;

		ImGui::SetCurrentContext(m_Context);
		ImGui::Initialize();

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = "Config/ImGui.ini";
		io.LogFilename = "Logs/ImGui.log";
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		if (createInfo.ViewportsEnable)
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		if (createInfo.DockingEnable)
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		AddFont(FontWeight::Regular, "Resources/Fonts/Roboto/Roboto-Regular.ttf");
		AddFont(FontWeight::Bold, "Resources/Fonts/Roboto/Roboto-Bold.ttf");
		AddFont(FontWeight::Light, "Resources/Fonts/Roboto/Roboto-Light.ttf");

		io.FontDefault = m_Fonts.at(FontWeight::Regular);

		InitDefaultStyle();

		ImGuiImplementationCreateInfo implementationCreateInfo;
		implementationCreateInfo.Context = m_Context;
		implementationCreateInfo.Window = createInfo.Window;

		m_Implementation = ImGuiImplementation::Create(implementationCreateInfo);

		if (m_PreviousContext)
		{
			ImGuiRenderer* previousRenderer = s_ContextMap.at(m_PreviousContext);
			if (!previousRenderer)
				LQ_PLATFORM_BREAK();

			ImGui::SetCurrentContext(m_PreviousContext);
		}
		else
		{
			ImGui::SetCurrentContext(nullptr);
		}
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		m_Implementation = nullptr;
		ImGui::DestroyContext(m_Context);
	}

	void ImGuiRenderer::BeginFrame()
	{
		m_PreviousContext = ImGui::GetCurrentContext();
		ImGui::SetCurrentContext(m_Context);

		m_Implementation->BeginFrame();

		ImGui::NewFrame();
	}

	void ImGuiRenderer::EndFrame()
	{
		if (ImGui::GetCurrentContext() != m_Context)
			LQ_PLATFORM_BREAK();

		ImGui::Render();

		m_Implementation->EndFrame();

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		if (m_PreviousContext)
		{
			ImGuiRenderer* previousRenderer = s_ContextMap.at(m_PreviousContext);
			if (!previousRenderer)
				LQ_PLATFORM_BREAK();

			ImGui::SetCurrentContext(m_PreviousContext);
		}
		else
		{
			ImGui::SetCurrentContext(nullptr);
		}
	}

	void ImGuiRenderer::AddFont(FontWeight weight, const String& filepath)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFont* font = io.Fonts->AddFontFromFileTTF(filepath.c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
		LQ_ASSERT(m_Fonts.find(weight) == m_Fonts.end(), "Font already exists!");
		m_Fonts[weight] = font;
	}

	void ImGuiRenderer::InitDefaultStyle()
	{ 
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

		auto& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		auto& colors = style.Colors;
		colors[ImGuiCol_Text]                  = ImVec4(0.917647f, 0.909804f, 0.917647f, 1.0f);
		colors[ImGuiCol_TextDisabled]          = ImVec4(0.513726f, 0.509804f, 0.513726f, 1.0f);
		colors[ImGuiCol_WindowBg]              = ImVec4(0.0862745f, 0.0784314f, 0.0862745f, 1.0f);
		colors[ImGuiCol_ChildBg]               = ImVec4(0.0627451f, 0.0627451f, 0.0627451f, 1.0f);
		colors[ImGuiCol_PopupBg]               = ImVec4(0.101961f, 0.0980392f, 0.105882f, 1.0f);
		colors[ImGuiCol_Border]                = ImVec4(0.203922f, 0.2f, 0.203922f, 1.0f);
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
		colors[ImGuiCol_Separator]             = ImVec4(0.196078f, 0.182238f, 0.195916f, 1.0f);
		colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.1f, 0.4f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive]       = ImVec4(0.1f, 0.4f, 0.75f, 1.0f);
		colors[ImGuiCol_ResizeGrip]            = ImVec4(0.2f, 0.760784f, 0.964706f, 0.784314f);
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