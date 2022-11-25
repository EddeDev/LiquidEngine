#include "LiquidPCH.h"
#include "DX11ImGuiImplementation.h"

#include "DX11Device.h"

#include "Liquid/Core/Application.h"

#include <backends/imgui_impl_glfw.cpp>
#include <backends/imgui_impl_dx11.cpp>
#include <GLFW/glfw3.h>
 
namespace Liquid {

	DX11ImGuiImplementation::DX11ImGuiImplementation()
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		ImGui_ImplGlfw_InitForOther(static_cast<GLFWwindow*>(Application::GetWindow()->GetHandle()), true);
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

}