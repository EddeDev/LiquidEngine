#pragma once

#include "Liquid/Launch/CommandLineArgs.h"

#include "Window/Window.h"

namespace Liquid {

	struct ApplicationCreateInfo
	{
		CommandLineArgs Args;
	};

	class Application
	{
	public:
		Application(const ApplicationCreateInfo& createInfo);
		~Application();

		void Run();
	private:
		ApplicationCreateInfo m_CreateInfo;
		Ref<Window> m_Window;
		bool m_Running = true;
	};

}