#include "LiquidPCH.h"

#include "CommandLineArgs.h"

#include "Liquid/Core/Application.h"

namespace Liquid {

	int32 Main(const CommandLineArgs& args)
	{
		Logger::Init();

		LQ_TRACE_ARGS("Liquid Engine");
		LQ_TRACE_ARGS("Initializing...");

		ApplicationCreateInfo createInfo;
		createInfo.Args = args;
		Application::Init(createInfo);

		Application::Run();

		LQ_TRACE_ARGS("Shutting down...");
		Application::Shutdown();
		Logger::Shutdown();
		return 0;
	}

}