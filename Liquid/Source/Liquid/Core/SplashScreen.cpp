#include "LiquidPCH.h"
#include "SplashScreen.h"

#include "Window/Window.h"

#include "Liquid/Renderer/API/Swapchain.h"
#include "Liquid/Renderer/API/Texture.h"
#include "Liquid/Renderer/ImGuiRenderer.h"

#include <imgui.h>

#include <GLFW/glfw3.h>

namespace Liquid {

	SplashScreen::SplashScreen()
	{
		m_Thread = std::thread(ThreadLoop, this);
	}

	SplashScreen::~SplashScreen()
	{
		m_Thread.join();
	}

	void SplashScreen::AddProgressData(const ProgressData& data)
	{
		std::scoped_lock<std::mutex> lock(m_ThreadMutex);

		m_ProgressList.push_back(data);
		glfwPostEmptyEvent();
	}

	void SplashScreen::ThreadLoop(SplashScreen* instance)
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Width = 1440 / 2;
		windowCreateInfo.Height = 740 / 2;
		windowCreateInfo.Resizable = false;
		windowCreateInfo.Decorated = false;
		Ref<Window> window = Window::Create(windowCreateInfo);

		SwapchainCreateInfo swapchainCreateInfo;
		swapchainCreateInfo.WindowHandle = window->GetPlatformHandle();
		swapchainCreateInfo.InitialWidth = window->GetWidth();
		swapchainCreateInfo.InitialHeight = window->GetHeight();
		swapchainCreateInfo.InitialFullscreenState = window->IsFullscreen();
		swapchainCreateInfo.InitialVSyncState = true;
		swapchainCreateInfo.AllowTearing = false;
		swapchainCreateInfo.ColorFormat = PixelFormat::RGBA;
		swapchainCreateInfo.DepthFormat = PixelFormat::DEPTH24_STENCIL8;
		swapchainCreateInfo.BufferCount = 3;
		swapchainCreateInfo.SampleCount = 1;
		Ref<Swapchain> swapchain = Swapchain::Create(swapchainCreateInfo);

		ImGuiRendererCreateInfo imguiRendererCreateInfo;
		imguiRendererCreateInfo.Window = window;
		imguiRendererCreateInfo.ViewportsEnable = false;
		imguiRendererCreateInfo.DebugName = "ImGuiRenderer-SplashScreen";
		Ref<ImGuiRenderer> imguiRenderer = Ref<ImGuiRenderer>::Create(imguiRendererCreateInfo);

		Ref<Texture> backgroundTexture = Texture::Create("Resources/Textures/SplashScreenImage.png");
		
		float progress = 0.0f;
		float lastProgress = -1.0f;

		window->SetVisible(true);

		while (progress < 100.0f)
		{
			window->WaitEvents();

			ProgressData currentProgress;
			{
				std::scoped_lock<std::mutex> lock(instance->m_ThreadMutex);
				currentProgress = instance->m_ProgressList.back();
			}

			progress = currentProgress.Progress;
			if (progress != lastProgress)
			{
				swapchain->BeginFrame();
				swapchain->Clear(BUFFER_COLOR | BUFFER_DEPTH);

				imguiRenderer->BeginFrame();

				BeginDockSpace();

				auto viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);

				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

				ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.01f, 0.01f, 0.02f, 1.0f });

				ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
				windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				windowFlags |= ImGuiWindowFlags_NoNavFocus;

				float cursorOffsetX = 8.0f;

				ImGui::Begin("SplashScreenWindow", nullptr, windowFlags);

				Ref<ImGuiImplementation> imgui = imguiRenderer->GetImplementation();
				imgui->Image(backgroundTexture, ImGui::GetContentRegionAvail());

				ImGui::SetCursorPosY(300.0f);

				ImFont* boldFont = imguiRenderer->GetFont(FontWeight::Bold);
				ImGui::PushFont(boldFont);
				ImGui::SetCursorPosX(cursorOffsetX);
				ImGui::TextUnformatted("Liquid Editor");
				ImGui::PopFont();

				ImFont* lightFont = imguiRenderer->GetFont(FontWeight::Light);
				ImGui::PushFont(lightFont);
				ImGui::SetCursorPosX(cursorOffsetX);
				ImGui::TextUnformatted("Liquid Editor");

				String info = fmt::format("{0}% - {1}", (uint32)currentProgress.Progress, currentProgress.InfoText);
				ImGui::SetCursorPosX(cursorOffsetX);
				ImGui::TextUnformatted(info.c_str());

				cursorOffsetX = 430;
				ImGui::SameLine();
				ImGui::SetCursorPosX(cursorOffsetX);
				ImGui::TextUnformatted("Copyright (c) EddeDev. All rights reserved.");

				ImGui::PopFont();

				ImGui::End();

				ImGui::PopStyleColor();
				ImGui::PopStyleVar(3);

				EndDockSpace();

				imguiRenderer->EndFrame();

				swapchain->Present();

				lastProgress = progress;
			}
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