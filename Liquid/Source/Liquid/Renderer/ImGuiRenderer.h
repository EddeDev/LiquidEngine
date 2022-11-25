#pragma once

namespace Liquid {

	class ImGuiRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();
	private:
		static void InitDefaultStyle();
	};

}