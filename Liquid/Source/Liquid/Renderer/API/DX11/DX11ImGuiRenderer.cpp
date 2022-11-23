#include "LiquidPCH.h"
#include "DX11ImGuiRenderer.h"

#include "DX11Device.h"

#include "Liquid/Core/Application.h"

#include <backends/imgui_impl_glfw.cpp>
#include <backends/imgui_impl_dx11.cpp>
#include <GLFW/glfw3.h>

namespace Liquid {

	void DX11ImGuiRenderer::Init()
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetWindow()->GetHandle());
		ImGui_ImplGlfw_InitForOther(window, true);

		ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
	}

#if 0
	void DX11ImGuiRenderer::Shutdown()
	{
		ImGui_ImplDX11_Shutdown();
	}
#endif

	void DX11ImGuiRenderer::NewFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplGlfw_NewFrame();
	}

	void DX11ImGuiRenderer::Render()
	{
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

}