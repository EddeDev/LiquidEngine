#pragma once

namespace Liquid {

	class SplashScreen
	{
	public:
		static void Init();
		static void Shutdown();

		static void Run();
	private:
		static void BeginDockSpace();
		static void EndDockSpace();
	};

}