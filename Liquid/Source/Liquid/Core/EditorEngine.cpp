#include "LiquidPCH.h"
#include "EditorEngine.h"

namespace Liquid {

	EditorEngine::EditorEngine(const EngineCreateInfo& createInfo)
		: Engine(createInfo)
	{
	}

	void EditorEngine::OnInit()
	{
		m_ThemeBuilder = CreateUnique<ThemeBuilder>();
	}

	void EditorEngine::OnShutdown()
	{
		m_ThemeBuilder = nullptr;
	}

	void EditorEngine::OnUpdate()
	{
	}

	void EditorEngine::OnUIRender()
	{
		m_ThemeBuilder->Render();

		ImGui::Begin("Liquid Engine");

		BuildConfiguration buildConfig = GetBuildConfiguration();
		String buildConfigName;
		switch (buildConfig)
		{
		case BuildConfiguration::Debug:    buildConfigName = "Debug"; break;
		case BuildConfiguration::Release:  buildConfigName = "Release"; break;
		case BuildConfiguration::Shipping: buildConfigName = "Shipping"; break;
		}
		ImGui::Text("Build Configuration: %s", buildConfigName.c_str());

		if (ImGui::CollapsingHeader("Graphics Device"))
		{
			auto& deviceInfo = Engine::GetDevice()->GetInfo();
			ImGui::Text("Vendor: %s", GraphicsDeviceUtils::VendorToString(deviceInfo.Vendor));
			// ImGui::Text("Renderer: %s", deviceInfo.Renderer.c_str());
			// String currentGraphicsAPI = GraphicsAPIUtils::GetGraphicsAPIName(GetGraphicsAPI());
			// ImGui::Text("%s Version: %s", currentGraphicsAPI.c_str(), deviceInfo.PlatformVersion.c_str());
		}

		Ref<Swapchain> swapchain = Engine::Get().GetMainWindow()->GetSwapchain();
		bool vsync = swapchain->IsVSyncEnabled();
		if (ImGui::Checkbox("V-Sync", &vsync))
			swapchain->SetVSync(vsync);

		ImGui::End();
	}

}