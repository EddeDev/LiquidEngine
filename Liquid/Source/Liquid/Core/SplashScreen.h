#pragma once

#include "Window/Window.h"
#include "Threading/Thread.h"

#include "Liquid/Renderer/API/Swapchain.h"
#include "Liquid/Renderer/API/Texture.h"
#include "Liquid/Renderer/ImGuiRenderer.h"

namespace Liquid {

	class SplashScreen : public RefCounted
	{
	public:
		struct ProgressData
		{
			String InfoText;
			float Progress;
		};
	public:
		SplashScreen();
		virtual ~SplashScreen();

		void AddProgressData(const ProgressData& data);
	private:
		struct ThreadData
		{
			Ref<Window> Window;
			Ref<Swapchain> Swapchain;
			Ref<ImGuiRenderer> ImGuiRenderer;
			Ref<Texture> BackgroundTexture;
			
			std::vector<ProgressData>* ProgressList;
		};
	private:
		static void ThreadInit(ThreadData* data);
		static void ThreadLoop(ThreadData* data);

		static void BeginDockSpace();
		static void EndDockSpace();
	private:
		Unique<Thread> m_Thread;
		ThreadData* m_ThreadData = nullptr;
		std::vector<ProgressData> m_ProgressList;
		static std::mutex s_Mutex;
	};

}