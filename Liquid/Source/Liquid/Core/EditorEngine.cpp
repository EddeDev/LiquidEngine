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
	}

	void EditorEngine::OnUpdate()
	{
	}

	void EditorEngine::OnUIRender()
	{
		m_ThemeBuilder->Render();

		static bool renderResourcesWindowOpen = true;
		if (renderResourcesWindowOpen)
		{
			ImGui::Begin("Render Resources", &renderResourcesWindowOpen);

			ImGui::Text("Reference count: %d", RenderResourceManager::GetResourceCount());

			auto& globalReferences = RenderResourceManager::GetGlobalReferences();
			for (const auto& [type, list] : globalReferences)
			{
				ImGui::PushID((int32)type);

				const char* label = RenderResourceUtils::RenderResourceTypeToString(type);
				if (ImGui::CollapsingHeader(label))
				{
					for (const auto& resource : list)
					{
						ImGui::PushID(&resource);

						String resourceStr = fmt::format("{0} {1}", label, (void*)resource);
						if (ImGui::TreeNodeEx(resourceStr.c_str()))
						{
							std::time_t time = resource->GetTime();
							std::tm* localTime = std::localtime(&time);

							char buffer[32];
							std::strftime(buffer, 32, "%Y.%m.%d %H:%M:%S", localTime);

							ImGui::Text("Created: %s", buffer);
							ImGui::Text("Reference Count: %d", resource->GetReferenceCount());

							ImGui::TreePop();
						}

						ImGui::PopID();
					}
				}

				ImGui::PopID();
			}

			ImGui::End();
		}

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