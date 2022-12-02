#pragma once

namespace Liquid {

	class SplashScreen
	{
	public:
		static void Show();
		static void Hide();

		static void SetProgress(uint8 progress);
		static void SetText(const String& text);

		static void SetProgress(uint8 progress, const String& text)
		{
			SetProgress(progress);
			SetText(text);
		}
	};

}