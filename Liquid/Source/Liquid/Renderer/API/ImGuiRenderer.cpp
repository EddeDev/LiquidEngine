#include "LiquidPCH.h"
#include "ImGuiRenderer.h"

#include "Liquid/Core/Application.h"

#include "DX11/DX11ImGuiRenderer.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Liquid {

	void ImGuiRenderer::InitContext()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf", 18.0f);

		ImGui::StyleColorsDark();

		auto& style = ImGui::GetStyle();
		style.FrameRounding = 3.0f;
		style.FrameBorderSize = 1.0f;
		style.WindowRounding = 3.0f;
		style.WindowBorderSize = 0.0f;
		style.GrabRounding = 0.5f;
		style.AntiAliasedLines = true;
		style.AntiAliasedFill = true;

		auto& colors = style.Colors;

		// Window BG
		colors[ImGuiCol_WindowBg] = { 0.09f, 0.08f, 0.09f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg]        = { 0.20f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = { 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_FrameBgActive]  = { 0.15f, 0.15f, 0.15f, 1.0f };

		// Title BG
		colors[ImGuiCol_TitleBg]          = { 0.15f, 0.14f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgActive]    = { 0.15f + 0.05f, 0.14f + 0.05f, 0.15f + 0.05f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = { 0.15f, 0.14f, 0.15f, 1.0f };

		// Header
		colors[ImGuiCol_Header]             = { 0.20f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_HeaderHovered]      = { 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_HeaderActive]       = { 0.15f, 0.15f, 0.15f, 1.0f };

		// Tab
		colors[ImGuiCol_Tab]                = { 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabHovered]         = { 0.38f, 0.38f, 0.38f, 1.0f };
		colors[ImGuiCol_TabActive]          = { 0.28f, 0.28f, 0.28f, 1.0f };
		colors[ImGuiCol_TabUnfocused]       = { 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = { 0.20f, 0.20f, 0.20f, 1.0f };

		// Button
		colors[ImGuiCol_Button]        = { 0.20f, 0.20f, 0.20f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = { 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = { 0.30f, 0.30f, 0.30f, 1.0f };
		colors[ImGuiCol_ButtonActive]  = { 0.15f, 0.15f, 0.15f, 1.0f };

		// Resize grip
		colors[ImGuiCol_ResizeGrip]        = { 0.23f, 0.77f, 0.98f, 0.8f };
		colors[ImGuiCol_ResizeGripActive]  = { 0.23f, 0.77f, 0.98f, 1.0f };
		colors[ImGuiCol_ResizeGripHovered] = { 0.23f, 0.77f, 0.98f, 0.9f };

		// Check mark
		colors[ImGuiCol_CheckMark] = { 0.25f, 0.58f, 0.70f, 1.0f };
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		// TODO: Shutdown();

		ImGui::DestroyContext();
	}

	void ImGuiRenderer::BeginFrame()
	{
		NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiRenderer::EndFrame()
	{
		ImGui::Render();
		Render();

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	Ref<ImGuiRenderer> ImGuiRenderer::Create()
	{
		Ref<ImGuiRenderer> result;
		switch (Application::GetGraphicsAPI())
		{
		case GraphicsAPI::DX11: result = Ref<DX11ImGuiRenderer>::Create(); break;
		}
		result->InitContext();
		result->Init();
		return result;
	}

}