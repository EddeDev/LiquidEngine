#include "LiquidPCH.h"
#include "DX11ImGuiImplementation.h"

#include "Liquid/Core/Engine.h"

#include "DX11Device.h"
#include "DX11Image.h"

#include <backends/imgui_impl_glfw.cpp>
#include <backends/imgui_impl_dx11.cpp>
#include <GLFW/glfw3.h>
 
namespace Liquid {

	DX11ImGuiImplementation::DX11ImGuiImplementation(const ImGuiImplementationCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		m_ThreadID = std::this_thread::get_id();

		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		GLFWwindow* window = static_cast<GLFWwindow*>(createInfo.Window->GetHandle());
		ImGui_ImplGlfw_InitForOther(window, false);
		InstallCallbacks();

		ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
	}

	DX11ImGuiImplementation::~DX11ImGuiImplementation()
	{
		ImGui_ImplDX11_Shutdown();
	}

	void DX11ImGuiImplementation::BeginFrame()
	{
		LQ_CHECK(std::this_thread::get_id() != m_ThreadID);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplGlfw_NewFrame();
	}

	void DX11ImGuiImplementation::EndFrame()
	{
		ImDrawData* drawData = ImGui::GetDrawData();
		LQ_CHECK(drawData->Valid);

		ImGui_ImplDX11_RenderDrawData(drawData);
	}

	void DX11ImGuiImplementation::Image(Ref<Image2D> image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1) const
	{
		LQ_CHECK(std::this_thread::get_id() != m_ThreadID);

		Ref<DX11Image2D> dxImage = image.As<DX11Image2D>();
		ID3D11ShaderResourceView* srv = dxImage->GetShaderResourceView();
		if (srv)
		{
			ImTextureID id = reinterpret_cast<ImTextureID>(srv);
			ImGui::Image(id, size, ImVec2(uv0.x, 1.0f - uv0.y), ImVec2(uv1.x, 1.0f - uv1.y));
		}
	}

	void DX11ImGuiImplementation::InstallCallbacks()
	{
		LQ_CHECK(std::this_thread::get_id() == m_ThreadID);

		m_CreateInfo.Window->AddFocusCallback(LQ_BIND_CALLBACK(OnWindowFocusCallback, this));
		m_CreateInfo.Window->AddCursorEnterCallback(LQ_BIND_CALLBACK(OnCursorEnterCallback, this));
		m_CreateInfo.Window->AddCursorPosCallback(LQ_BIND_CALLBACK(OnCursorPosCallback, this));
		m_CreateInfo.Window->AddMouseButtonCallback(LQ_BIND_CALLBACK(OnMouseButtonCallback, this));
		m_CreateInfo.Window->AddScrollCallback(LQ_BIND_CALLBACK(OnScrollCallback, this));
		m_CreateInfo.Window->AddKeyCallback(LQ_BIND_CALLBACK(OnKeyCallback, this));
		m_CreateInfo.Window->AddCharCallback(LQ_BIND_CALLBACK(OnCharCallback, this));
	}

#define IMGUI_CALLBACK(name, ...) \
{ \
	LQ_ASSERT(std::this_thread::get_id() == m_ThreadID, "On" #name " must only be called from the main thread"); \
	Engine::Get().SubmitToUpdateThread([createInfo = m_CreateInfo, __VA_ARGS__]() \
	{ \
		ImGui::SetCurrentContext(createInfo.Context); \
		GLFWwindow* window = static_cast<GLFWwindow*>(createInfo.Window->GetHandle()); \
		LQ_ASSERT(window, "Window is null"); \
		ImGui_ImplGlfw_##name(window, __VA_ARGS__); \
	}); \
}

	void DX11ImGuiImplementation::OnWindowFocusCallback(int32 focused)
	{
		IMGUI_CALLBACK(WindowFocusCallback, focused);
	}

	void DX11ImGuiImplementation::OnCursorEnterCallback(int32 entered)
	{
		IMGUI_CALLBACK(CursorEnterCallback, entered);
	}

	void DX11ImGuiImplementation::OnCursorPosCallback(double x, double y)
	{
		IMGUI_CALLBACK(CursorPosCallback, x, y);
	}

	void DX11ImGuiImplementation::OnMouseButtonCallback(int32 button, int32 action, int32 mods)
	{
		IMGUI_CALLBACK(MouseButtonCallback, button, action, mods);
	}

	void DX11ImGuiImplementation::OnScrollCallback(double xoffset, double yoffset)
	{
		IMGUI_CALLBACK(ScrollCallback, xoffset, yoffset);
	}

	void DX11ImGuiImplementation::OnKeyCallback(int32 key, int32 scancode, int32 action, int32 mods)
	{
		IMGUI_CALLBACK(KeyCallback, key, scancode, action, mods);
	}

	void DX11ImGuiImplementation::OnCharCallback(uint32 codepoint)
	{
		IMGUI_CALLBACK(CharCallback, codepoint);
	}

}