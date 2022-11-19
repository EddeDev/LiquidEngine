#pragma once

#include "Liquid/Launch/CommandLineArgs.h"

#include "Window/Window.h"

namespace Liquid {

	enum class BuildConfiguration : uint8
	{
		None = 0,

		Debug,
		Release,
		Shipping
	};

	enum class BuildTargetType : uint8
	{
		None = 0,

		Game,
		Editor
	};

	struct ApplicationCreateInfo
	{
		CommandLineArgs Args;
	};

	class Application
	{
	public:
		static void Init(const ApplicationCreateInfo& createInfo);
		static void Shutdown();

		static void Run();

		static BuildConfiguration GetBuildConfiguration();
	private:
		static Ref<Window> s_Window;
		static bool s_Running;
	};

}