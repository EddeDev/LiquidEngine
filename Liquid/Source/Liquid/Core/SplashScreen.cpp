#include "LiquidPCH.h"
#include "SplashScreen.h"

#include "Window/Window.h"

#include "Liquid/Renderer/API/Swapchain.h"
#include "Liquid/Renderer/API/Texture.h"
#include "Liquid/Renderer/ImGuiRenderer.h"

#include <imgui.h>

#include <GLFW/glfw3.h>






// Temp
#include <random>

namespace Liquid {

	struct SplashScreenData
	{
		Ref<Window> Window;
		Ref<Swapchain> Swapchain;
		Ref<ImGuiRenderer> ImGuiRenderer;

		Ref<Texture> BackgroundTexture;

		bool Running;
	};

	static SplashScreenData* s_Data = nullptr;

	void SplashScreen::Init()
	{
		s_Data = new SplashScreenData();
		memset(s_Data, 0, sizeof(SplashScreenData));

		// Window
		{
			WindowCreateInfo windowCreateInfo;
			windowCreateInfo.Width = 1440 / 2;
			windowCreateInfo.Height = 740 / 2;
			windowCreateInfo.Resizable = false;
			windowCreateInfo.Decorated = false;

			s_Data->Window = Window::Create(windowCreateInfo);
		}

		// Swapchain
		{
			SwapchainCreateInfo swapchainCreateInfo;
			swapchainCreateInfo.WindowHandle = s_Data->Window->GetPlatformHandle();
			swapchainCreateInfo.InitialWidth = s_Data->Window->GetWidth();
			swapchainCreateInfo.InitialHeight = s_Data->Window->GetHeight();
			swapchainCreateInfo.InitialFullscreenState = s_Data->Window->IsFullscreen();
			swapchainCreateInfo.InitialVSyncState = true;
			swapchainCreateInfo.AllowTearing = false;
			swapchainCreateInfo.ColorFormat = PixelFormat::RGBA;
			swapchainCreateInfo.DepthFormat = PixelFormat::DEPTH24_STENCIL8;
			swapchainCreateInfo.BufferCount = 3;
			swapchainCreateInfo.SampleCount = 1;
			s_Data->Swapchain = Swapchain::Create(swapchainCreateInfo);
		}

		// ImGui
		{
			ImGuiRendererCreateInfo createInfo;
			createInfo.WindowHandle = s_Data->Window->GetHandle();
			createInfo.ViewportsEnable = false;

			s_Data->ImGuiRenderer = Ref<ImGuiRenderer>::Create(createInfo);
		}

		s_Data->BackgroundTexture = Texture::Create("Resources/Textures/SplashScreenImage.png");

		s_Data->Window->SetVisible(true);
		s_Data->Running = true;
	}

	void SplashScreen::Shutdown()
	{
		delete s_Data;
		s_Data = nullptr;
	}

	void SplashScreen::Run()
	{
		float lastTime = static_cast<float>(glfwGetTime());
		float deltaTime = 0.0f;

		float progress = 0.0f;

		while (s_Data->Running)
		{
			float time = static_cast<float>(glfwGetTime());
			deltaTime = time - lastTime;
			lastTime = time;

			s_Data->Window->PollEvents();

			s_Data->Swapchain->BeginFrame();
			s_Data->Swapchain->Clear(BUFFER_COLOR | BUFFER_DEPTH);

			s_Data->ImGuiRenderer->BeginFrame();
			
			BeginDockSpace();

			auto viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

			ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.01f, 0.01f, 0.02f, 1.0f });

			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoNavFocus;

			ImGui::Begin("SplashScreenWindow", nullptr, windowFlags);

			Ref<ImGuiImplementation> imgui = s_Data->ImGuiRenderer->GetImplementation();
			imgui->Image(s_Data->BackgroundTexture, ImGui::GetContentRegionAvail());

			ImGui::SetCursorPosY(300.0f);

			ImFont* boldFont = s_Data->ImGuiRenderer->GetFont(FontWeight::Bold);
			ImGui::PushFont(boldFont);
			ImGui::Text("Liquid Editor");
			ImGui::PopFont();

			ImFont* lightFont = s_Data->ImGuiRenderer->GetFont(FontWeight::Light);
			ImGui::PushFont(lightFont);
			ImGui::Text("Liquid Editor");

			// TEST
			float random = (float)std::rand() / (float)RAND_MAX;
			random *= 30.0f;
			progress += deltaTime * random;
			if (progress > 100.0f)
				s_Data->Running = false;

			String info = fmt::format("{0}% - Initializing...", (uint32)progress);
			ImGui::TextUnformatted(info.c_str());
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(430.0f);
			ImGui::TextUnformatted("Copyright (c) EddeDev. All rights reserved.");

			ImGui::PopFont();

			ImGui::End();

			ImGui::PopStyleColor();
			ImGui::PopStyleVar(2);

			EndDockSpace();

			s_Data->ImGuiRenderer->EndFrame();

			s_Data->Swapchain->Present();
		}
	}

	void SplashScreen::BeginDockSpace()
	{
		auto viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("SplashScreen-DockSpace", nullptr, windowFlags);
		ImGui::PopStyleVar(3);

		auto& io = ImGui::GetIO();
		ImGui::DockSpace(ImGui::GetID("SplashScreen-DockSpace"), ImVec2(0.0f, 0.0f));
	}

	void SplashScreen::EndDockSpace()
	{
		ImGui::End();
	}

}