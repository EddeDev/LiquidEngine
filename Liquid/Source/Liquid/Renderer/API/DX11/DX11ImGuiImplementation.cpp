#include "LiquidPCH.h"
#include "DX11ImGuiImplementation.h"

#include "DX11Device.h"
#include "DX11Texture.h"

#include <backends/imgui_impl_glfw.cpp>
#include <backends/imgui_impl_dx11.cpp>
#include <GLFW/glfw3.h>
 
namespace Liquid {

	DX11ImGuiImplementation::DX11ImGuiImplementation(const ImGuiImplementationCreateInfo& createInfo)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		ImGui_ImplGlfw_InitForOther(static_cast<GLFWwindow*>(createInfo.WindowHandle), true);
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

}