#include "LiquidPCH.h"
#include "SplashScreen.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

namespace Liquid {

	std::mutex SplashScreen::s_Mutex;

	SplashScreen::SplashScreen()
	{
		ThreadCreateInfo createInfo;
		createInfo.Name = "Splash Screen";
		createInfo.Priority = ThreadPriority::BelowNormal;

		m_Thread = CreateUnique<Thread>(createInfo);

		m_ThreadData = new ThreadData();
		memset(m_ThreadData, 0, sizeof(ThreadData));

		m_ThreadData->ProgressList = &m_ProgressList;

		m_Thread->PushJob([threadData = m_ThreadData]()
		{
			ThreadInit(threadData);
		});

		m_Thread->Wait();

		m_Thread->PushJob([threadData = m_ThreadData]()
		{
			ThreadLoop(threadData);
		});

		// HACK
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1000ms);
	}

	SplashScreen::~SplashScreen()
	{
		m_Thread->Join();

		delete m_ThreadData;
		m_ThreadData = nullptr;
	}

	void SplashScreen::AddProgressData(const ProgressData& data)
	{
		std::scoped_lock<std::mutex> lock(s_Mutex);
		m_ProgressList.push_back(data);
		glfwPostEmptyEvent();
	}

	void SplashScreen::ThreadInit(ThreadData* data)
	{
		// Window
		{
			WindowCreateInfo windowCreateInfo;
			windowCreateInfo.Width = 1440 / 2;
			windowCreateInfo.Height = 740 / 2;
			windowCreateInfo.Resizable = false;
			windowCreateInfo.Decorated = false;
			data->Window = Window::Create(windowCreateInfo);
		}
	
		// Swapchain
		{
			SwapchainCreateInfo swapchainCreateInfo;
			swapchainCreateInfo.WindowHandle = data->Window->GetPlatformHandle();
			swapchainCreateInfo.InitialWidth = data->Window->GetWidth();
			swapchainCreateInfo.InitialHeight = data->Window->GetHeight();
			swapchainCreateInfo.InitialFullscreenState = data->Window->IsFullscreen();
			swapchainCreateInfo.InitialVSyncState = true;
			swapchainCreateInfo.AllowTearing = false;
			swapchainCreateInfo.ColorFormat = PixelFormat::RGBA;
			swapchainCreateInfo.DepthFormat = PixelFormat::DEPTH24_STENCIL8;
			swapchainCreateInfo.BufferCount = 3;
			swapchainCreateInfo.SampleCount = 1;
			data->Swapchain = Swapchain::Create(swapchainCreateInfo);
		}
	
		// ImGui
		{
			ImGuiRendererCreateInfo imguiRendererCreateInfo;
			imguiRendererCreateInfo.Window = data->Window;
			imguiRendererCreateInfo.ViewportsEnable = false;
			imguiRendererCreateInfo.DebugName = "ImGuiRenderer-SplashScreen";
			data->ImGuiRenderer = Ref<ImGuiRenderer>::Create(imguiRendererCreateInfo);
		}
	
		// Textures
		{
			data->BackgroundTexture = Texture::Create("Resources/Textures/SplashScreenImage.png");
		}
	}

	void SplashScreen::ThreadLoop(ThreadData* data)
	{
		float progress = 0.0f;
		float lastProgress = -1.0f;

		data->Window->SetVisible(true);

		while (progress < 100.0f)
		{
			data->Window->WaitEvents();

			ProgressData currentProgress = {};
			{
				std::scoped_lock<std::mutex> lock(s_Mutex);
				if (!data->ProgressList->empty())
					currentProgress = data->ProgressList->back();
			}

			progress = currentProgress.Progress;
			if (progress != lastProgress)
			{
				data->Swapchain->BeginFrame();
				data->Swapchain->Clear(BUFFER_COLOR | BUFFER_DEPTH);

				data->ImGuiRenderer->BeginFrame();

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

				Ref<ImGuiImplementation> imgui = data->ImGuiRenderer->GetImplementation();
				imgui->Image(data->BackgroundTexture, ImGui::GetContentRegionAvail());

				ImGui::SetCursorPosY(300.0f);

				ImFont* boldFont = data->ImGuiRenderer->GetFont(FontWeight::Bold);
				ImGui::PushFont(boldFont);
				ImGui::SetCursorPosX(cursorOffsetX);
				ImGui::TextUnformatted("Liquid Editor");
				ImGui::PopFont();

				ImFont* lightFont = data->ImGuiRenderer->GetFont(FontWeight::Light);
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

				data->ImGuiRenderer->EndFrame();

				data->Swapchain->Present();

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