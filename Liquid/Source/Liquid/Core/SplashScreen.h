#pragma once

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
		static void ThreadLoop(SplashScreen* instance);

		static void BeginDockSpace();
		static void EndDockSpace();
	private:
		std::thread m_Thread;
		std::mutex m_ThreadMutex;

		std::vector<ProgressData> m_ProgressList;
	};

}