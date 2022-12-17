#include "LiquidPCH.h"

#include "CommandLineArgs.h"

// #define LQ_EDITOR
#define LQ_RUNTIME

#ifdef LQ_EDITOR
#include "Liquid/Core/EditorEngine.h"
#endif

#ifdef LQ_RUNTIME
#include "Liquid/Core/GameEngine.h"
#endif

namespace Liquid {

	Engine* CreateEngine(const CommandLineArgs& args)
	{
#ifdef LQ_EDITOR
		EngineCreateInfo createInfo;
		createInfo.Args = args;
		createInfo.EnableImGui = true;
		createInfo.ShowSplashScreen = true;

		return new EditorEngine(createInfo);
#endif

#ifdef LQ_RUNTIME
		EngineCreateInfo createInfo;
		createInfo.Args = args;
		createInfo.EnableImGui = false;
		createInfo.ShowSplashScreen = false;
		createInfo.MaximizeOnStart = false;

		return new GameEngine(createInfo);
#endif

		return nullptr;
	}

	int32 Main(const CommandLineArgs& args)
	{
		Logger::Init();

		LQ_TRACE_ARGS("Liquid Engine");
		LQ_TRACE_ARGS("Initializing...");

		Engine* engine = CreateEngine(args);
		LQ_VERIFY(engine, "Engine is nullptr!");

		engine->Run();

		LQ_TRACE_ARGS("Shutting down...");
		delete engine;

		Logger::Shutdown();
		return 0;
	}

}