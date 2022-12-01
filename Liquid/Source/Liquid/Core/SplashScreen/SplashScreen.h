#pragma once

namespace Liquid {

	struct SplashScreenCreateInfo
	{
		uint32 Width = 1440 / 2;
		uint32 Height = 720 / 2;
	};

	class SplashScreen : public RefCounted
	{
	public:
		virtual ~SplashScreen() {}

		static Ref<SplashScreen> Create(const SplashScreenCreateInfo& createInfo);
	};

}