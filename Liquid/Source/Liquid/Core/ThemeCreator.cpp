#include "LiquidPCH.h"
#include "ThemeCreator.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Liquid {

	static const char* s_StyleStrings[ImGuiCol_COUNT] = {
			"ImGuiStyleVar_Alpha",
			"ImGuiStyleVar_DisabledAlpha",
			"ImGuiStyleVar_WindowPadding",
			"ImGuiStyleVar_WindowRounding",
			"ImGuiStyleVar_WindowBorderSize",
			"ImGuiStyleVar_WindowMinSize",
			"ImGuiStyleVar_WindowTitleAlign",
			"ImGuiStyleVar_ChildRounding",
			"ImGuiStyleVar_ChildBorderSize",
			"ImGuiStyleVar_PopupRounding",
			"ImGuiStyleVar_PopupBorderSize",
			"ImGuiStyleVar_FramePadding",
			"ImGuiStyleVar_FrameRounding",
			"ImGuiStyleVar_FrameBorderSize",
			"ImGuiStyleVar_ItemSpacing",
			"ImGuiStyleVar_ItemInnerSpacing",
			"ImGuiStyleVar_IndentSpacing",
			"ImGuiStyleVar_CellPadding",
			"ImGuiStyleVar_ScrollbarSize",
			"ImGuiStyleVar_ScrollbarRounding",
			"ImGuiStyleVar_GrabMinSize",
			"ImGuiStyleVar_GrabRounding",
			"ImGuiStyleVar_TabRounding",
			"ImGuiStyleVar_ButtonTextAlign",
			"ImGuiStyleVar_SelectableTextAlign"
	};

	static const char* s_ColorStrings[ImGuiCol_COUNT] = {
			"ImGuiCol_Text",
			"ImGuiCol_TextDisabled",
			"ImGuiCol_WindowBg",
			"ImGuiCol_ChildBg",
			"ImGuiCol_PopupBg",
			"ImGuiCol_Border",
			"ImGuiCol_BorderShadow",
			"ImGuiCol_FrameBg",
			"ImGuiCol_FrameBgHovered",
			"ImGuiCol_FrameBgActive",
			"ImGuiCol_TitleBg",
			"ImGuiCol_TitleBgActive",
			"ImGuiCol_TitleBgCollapsed",
			"ImGuiCol_MenuBarBg",
			"ImGuiCol_ScrollbarBg",
			"ImGuiCol_ScrollbarGrab",
			"ImGuiCol_ScrollbarGrabHovered",
			"ImGuiCol_ScrollbarGrabActive",
			"ImGuiCol_CheckMark",
			"ImGuiCol_SliderGrab",
			"ImGuiCol_SliderGrabActive",
			"ImGuiCol_Button",
			"ImGuiCol_ButtonHovered",
			"ImGuiCol_ButtonActive",
			"ImGuiCol_Header",
			"ImGuiCol_HeaderHovered",
			"ImGuiCol_HeaderActive",
			"ImGuiCol_Separator",
			"ImGuiCol_SeparatorHovered",
			"ImGuiCol_SeparatorActive",
			"ImGuiCol_ResizeGrip",
			"ImGuiCol_ResizeGripHovered",
			"ImGuiCol_ResizeGripActive",
			"ImGuiCol_Tab",
			"ImGuiCol_TabHovered",
			"ImGuiCol_TabActive",
			"ImGuiCol_TabUnfocused",
			"ImGuiCol_TabUnfocusedActive",
			"ImGuiCol_DockingPreview",
			"ImGuiCol_DockingEmptyBg",
			"ImGuiCol_PlotLines",
			"ImGuiCol_PlotLinesHovered",
			"ImGuiCol_PlotHistogram",
			"ImGuiCol_PlotHistogramHovered",
			"ImGuiCol_TableHeaderBg",
			"ImGuiCol_TableBorderStrong",
			"ImGuiCol_TableBorderLight",
			"ImGuiCol_TableRowBg",
			"ImGuiCol_TableRowBgAlt",
			"ImGuiCol_TextSelectedBg",
			"ImGuiCol_DragDropTarget",
			"ImGuiCol_NavHighlight",
			"ImGuiCol_NavWindowingHighlight",
			"ImGuiCol_NavWindowingDimBg",
			"ImGuiCol_ModalWindowDimBg"
	};

	// From imgui.cpp

	struct ImGuiStyleVarInfo
	{
		ImGuiDataType   Type;
		ImU32           Count;
		ImU32           Offset;
		void* GetVarPtr(ImGuiStyle* style) const { return (void*)((unsigned char*)style + Offset); }
	};

	static const ImGuiStyleVarInfo GStyleVarInfo[] =
	{
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, Alpha) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, DisabledAlpha) },
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowPadding) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowRounding) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowBorderSize) },
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowMinSize) },
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, WindowTitleAlign) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ChildRounding) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ChildBorderSize) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, PopupRounding) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, PopupBorderSize) },
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, FramePadding) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, FrameRounding) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, FrameBorderSize) },
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, ItemSpacing) },
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, ItemInnerSpacing) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, IndentSpacing) },
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, CellPadding) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ScrollbarSize) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, ScrollbarRounding) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, GrabMinSize) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, GrabRounding) },
		{ ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImGuiStyle, TabRounding) },
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, ButtonTextAlign) },
		{ ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImGuiStyle, SelectableTextAlign) }
	};

	static const ImGuiStyleVarInfo* GetStyleVarInfo(ImGuiStyleVar idx)
	{
		IM_ASSERT(idx >= 0 && idx < ImGuiStyleVar_COUNT);
		IM_ASSERT(IM_ARRAYSIZE(GStyleVarInfo) == ImGuiStyleVar_COUNT);
		return &GStyleVarInfo[idx];
	}

	void ThemeCreator::Render()
	{
		ImGui::Begin("Theme Creator");

		auto& style = ImGui::GetStyle();
		auto& colors = style.Colors;

		if (ImGui::CollapsingHeader("Sizes"))
		{
			// From imgui_demo.cpp
			ImGui::Text("Main");
			ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
			ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
			ImGui::Text("Borders");
			ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
			ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
			ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
			ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
			ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
			ImGui::Text("Rounding");
			ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");
			ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
			ImGui::Text("Alignment");
			ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
			int32 window_menu_button_position = style.WindowMenuButtonPosition + 1;
			if (ImGui::Combo("WindowMenuButtonPosition", (int32*)&window_menu_button_position, "None\0Left\0Right\0"))
				style.WindowMenuButtonPosition = window_menu_button_position - 1;
			ImGui::Combo("ColorButtonPosition", (int32*)&style.ColorButtonPosition, "Left\0Right\0");
			ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
			ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
			ImGui::Text("Safe Area Padding");
			ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
		}

		if (ImGui::CollapsingHeader("Colors"))
		{
			for (uint32 i = 0; i < ImGuiCol_COUNT; i++)
			{
				String label = s_ColorStrings[i];
				label = label.substr(strlen("ImGuiCol_"));

				ImGui::PushID(i);
				ImGui::ColorEdit4("##Color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar);
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
				ImGui::TextUnformatted(label.c_str());
				ImGui::PopID();
			}
		}

		if (ImGui::Button("Create"))
		{
			m_ShowOutput = true;
			m_NeedsUpdate = true;
		}

		if (m_ShowOutput)
		{
			if (m_NeedsUpdate)
			{
				auto& style = ImGui::GetStyle();

				m_CodeBuffer = std::stringstream();

				m_CodeBuffer << "auto& style = ImGui::GetStyle();\n";

				for (uint32 i = 0; i < ImGuiStyleVar_COUNT; i++)
				{
					auto& info = *GetStyleVarInfo(i);

					String nameStr = s_StyleStrings[i];
					nameStr = nameStr.substr(strlen("ImGuiStyleVar_"));

					m_CodeBuffer << "style.";
					m_CodeBuffer << nameStr;
					m_CodeBuffer << " = ";

					if (info.Count > 1)
						m_CodeBuffer << "{ ";

					const float* data = static_cast<float*>(info.GetVarPtr(&style));
					for (uint32 paramIndex = 0; paramIndex < info.Count; paramIndex++)
					{
						if (paramIndex != 0)
							m_CodeBuffer << ", ";

						float value = data[paramIndex];
						m_CodeBuffer << value;
						m_CodeBuffer << (value == (int32)value ? ".0f" : "f");
					}

					if (info.Count > 1)
						m_CodeBuffer << " }";

					m_CodeBuffer << ";\n";
				}

				m_CodeBuffer << "\n";
				m_CodeBuffer << "auto& colors = style.Colors;\n";

				std::vector<uint32> colorStringLengths(ImGuiCol_COUNT);
				for (uint32 i = 0; i < ImGuiCol_COUNT; i++)
					colorStringLengths[i] = strlen(s_ColorStrings[i]);

				uint32 highestColorStringLength = *std::max_element(colorStringLengths.begin(), colorStringLengths.end());

				for (uint32 i = 0; i < ImGuiCol_COUNT; i++)
				{
					ImVec4 color = style.Colors[i];

					String indent;
					for (uint32 y = 0; y < highestColorStringLength - strlen(s_ColorStrings[i]); y++)
						indent += " ";

					m_CodeBuffer << "colors[";
					m_CodeBuffer << s_ColorStrings[i];
					m_CodeBuffer << "] ";
					m_CodeBuffer << indent;
					m_CodeBuffer << "= ImVec4(";

					m_CodeBuffer << color.x;
					m_CodeBuffer << (color.x == (int32)color.x ? ".0f, " : "f, ");
					m_CodeBuffer << color.y;
					m_CodeBuffer << (color.y == (int32)color.y ? ".0f, " : "f, ");
					m_CodeBuffer << color.z;
					m_CodeBuffer << (color.z == (int32)color.z ? ".0f, " : "f, ");
					m_CodeBuffer << color.w;
					m_CodeBuffer << (color.w == (int32)color.w ? ".0f" : "f");

					m_CodeBuffer << ");\n";
				}

				m_NeedsUpdate = false;
			}

			ImGui::TextUnformatted("To use this theme, paste this code in ImGuiRenderer.cpp (line 28)");
			ImGui::SameLine(ImGui::GetItemRectSize().x + 20.0f);
			ImGui::TextColored({ 0.6f, 0.1f, 0.1f, 1.0f }, "NOTE: This code will be properly formatted/indented when copied.");

			String code = m_CodeBuffer.str();

			if (ImGui::Button("Copy to clipboard"))
			{
				ImGui::LogToClipboard();
				ImGui::LogText(code.c_str());
				ImGui::LogFinish();
			}

			ImGui::PushStyleColor(ImGuiCol_Text, { 0.95f, 0.95f, 1.0f, 1.0f });
			ImGui::InputTextMultiline("##ThemeCode", code.data(), code.size(), ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopStyleColor();
		}

		ImGui::End();
	}

}