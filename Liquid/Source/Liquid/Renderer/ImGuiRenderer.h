#pragma once

#include "API/ImGuiImplementation.h"

#include <imgui_internal.h>

namespace Liquid {

	struct ImGuiRendererCreateInfo
	{
		void* WindowHandle = nullptr;

		bool ViewportsEnable = true;
		bool DockingEnable = true;
	};

	enum class FontWeight : uint8
	{
		Regular = 0,
		Bold,
		Light
	};

	class ImGuiRenderer : public RefCounted
	{
	public:
		ImGuiRenderer(const ImGuiRendererCreateInfo& createInfo);
		virtual ~ImGuiRenderer();

		void BeginFrame();
		void EndFrame();

		ImFont* GetFont(FontWeight weight) const { return m_Fonts.at(weight); }

		Ref<ImGuiImplementation> GetImplementation() const { return m_Implementation; }
	private:
		void AddFont(FontWeight weight, const String& filepath);

		static void InitDefaultStyle();
	private:
		ImGuiContext* m_Context = nullptr;
		ImGuiContext* m_PreviousContext = nullptr;

		Ref<ImGuiImplementation> m_Implementation;

		std::unordered_map<FontWeight, ImFont*> m_Fonts;
	};

}