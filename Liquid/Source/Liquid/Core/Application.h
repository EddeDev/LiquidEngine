#pragma once

#include "Liquid/Launch/CommandLineArgs.h"

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
	};

}