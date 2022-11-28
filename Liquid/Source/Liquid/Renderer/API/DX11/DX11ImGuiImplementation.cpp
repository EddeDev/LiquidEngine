#include "LiquidPCH.h"
#include "DX11ImGuiImplementation.h"

#include "DX11Device.h"
#include "DX11Texture.h"

#include <backends/imgui_impl_glfw.cpp>
#include <backends/imgui_impl_dx11.cpp>
#include <GLFW/glfw3.h>
 
namespace Liquid {

	DX11ImGuiImplementation::DX11ImGuiImplementation(const ImGuiImplementationCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
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
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplGlfw_NewFrame();
	}

	void DX11ImGuiImplementation::EndFrame()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void DX11ImGuiImplementation::Image(Ref<Texture> texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1) const
	{
		Ref<DX11Texture> dxTexture = texture.As<DX11Texture>();
		ID3D11ShaderResourceView* srv = dxTexture->GetShaderResourceView();
		if (srv)
		{
			ImTextureID id = reinterpret_cast<ImTextureID>(srv);
			ImGui::Image(id, size, ImVec2(uv0.x, 1.0f - uv0.y), ImVec2(uv1.x, 1.0f - uv1.y));
		}
	}

	void DX11ImGuiImplementation::InstallCallbacks()
	{
		m_CreateInfo.Window->AddFocusCallback(LQ_BIND_CALLBACK(OnWindowFocusCallback, this));
		m_CreateInfo.Window->AddCursorEnterCallback(LQ_BIND_CALLBACK(OnCursorEnterCallback, this));
		m_CreateInfo.Window->AddCursorPosCallback(LQ_BIND_CALLBACK(OnCursorPosCallback, this));
		m_CreateInfo.Window->AddMouseButtonCallback(LQ_BIND_CALLBACK(OnMouseButtonCallback, this));
		m_CreateInfo.Window->AddScrollCallback(LQ_BIND_CALLBACK(OnScrollCallback, this));
		m_CreateInfo.Window->AddKeyCallback(LQ_BIND_CALLBACK(OnKeyCallback, this));
		m_CreateInfo.Window->AddCharCallback(LQ_BIND_CALLBACK(OnCharCallback, this));
	}

	void DX11ImGuiImplementation::OnWindowFocusCallback(int32 focused)
	{
		ImGui::SetCurrentContext(m_CreateInfo.Context);

		GLFWwindow* window = static_cast<GLFWwindow*>(m_CreateInfo.Window->GetHandle());
		ImGui_ImplGlfw_WindowFocusCallback(window, focused);
	}

	void DX11ImGuiImplementation::OnCursorEnterCallback(int32 entered)
	{
		ImGui::SetCurrentContext(m_CreateInfo.Context);

		GLFWwindow* window = static_cast<GLFWwindow*>(m_CreateInfo.Window->GetHandle());
		ImGui_ImplGlfw_CursorEnterCallback(window, entered);
	}

	void DX11ImGuiImplementation::OnCursorPosCallback(double x, double y)
	{
		ImGui::SetCurrentContext(m_CreateInfo.Context);

		GLFWwindow* window = static_cast<GLFWwindow*>(m_CreateInfo.Window->GetHandle());
		ImGui_ImplGlfw_CursorPosCallback(window, x, y);
	}

	void DX11ImGuiImplementation::OnMouseButtonCallback(int32 button, int32 action, int32 mods)
	{
		ImGui::SetCurrentContext(m_CreateInfo.Context);

		GLFWwindow* window = static_cast<GLFWwindow*>(m_CreateInfo.Window->GetHandle());
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	}

	void DX11ImGuiImplementation::OnScrollCallback(double xoffset, double yoffset)
	{
		ImGui::SetCurrentContext(m_CreateInfo.Context);

		GLFWwindow* window = static_cast<GLFWwindow*>(m_CreateInfo.Window->GetHandle());
		ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	}

	void DX11ImGuiImplementation::OnKeyCallback(int32 key, int32 scancode, int32 action, int32 mods)
	{
		ImGui::SetCurrentContext(m_CreateInfo.Context);

		GLFWwindow* window = static_cast<GLFWwindow*>(m_CreateInfo.Window->GetHandle());
		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	}

	void DX11ImGuiImplementation::OnCharCallback(uint32 codepoint)
	{
		ImGui::SetCurrentContext(m_CreateInfo.Context);

		GLFWwindow* window = static_cast<GLFWwindow*>(m_CreateInfo.Window->GetHandle());
		ImGui_ImplGlfw_CharCallback(window, codepoint);
	}

}