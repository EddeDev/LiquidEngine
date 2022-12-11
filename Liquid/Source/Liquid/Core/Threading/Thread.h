#pragma once

#include "Liquid/Core/String.h"

namespace Liquid {

	enum class ThreadPriority : int8
	{
		Lowest = -2,
		BelowNormal = -1,
		Normal = 0,
		AboveNormal = 1,
		Highest = 2
	};

	struct ThreadCreateInfo
	{
		String Name = "Thread";
		ThreadPriority Priority = ThreadPriority::Normal;
	};

	class Thread
	{
	public:
		using Job = std::function<void()>;
	public:
		Thread(const ThreadCreateInfo& createInfo);
		~Thread();

		void Join();
		void Wait();

		void PushJob(StringView description, Job job);

		std::thread::id GetThreadID() const { return m_Thread.get_id(); }
	private:
		void Destroy();

		void ExecuteJobs();
		Job* GetNextJob();
		void PopJob();
	private:
		ThreadCreateInfo m_CreateInfo;
		std::thread m_Thread;

		std::queue<Job> m_Jobs;
		std::mutex m_Mutex;
		std::condition_variable m_Condition;
		bool m_Destroying = false;
	};

	namespace ThreadUtils {

		static void* CurrentThreadHandle()
		{
#ifdef LQ_PLATFORM_WINDOWS
			return GetCurrentThread();
#else
			static_assert(false, "Unknown platform");
			return nullptr;
#endif
		}

		static void SetName(void* threadHandle, const String& name)
		{
#ifdef LQ_PLATFORM_WINDOWS
			auto threadName = StringUtils::ToWideString(name);
			SetThreadDescription(threadHandle, threadName.c_str());
#endif
		}

		static void SetPriority(void* threadHandle, ThreadPriority priority)
		{
#ifdef LQ_PLATFORM_WINDOWS
			SetThreadPriority(threadHandle, static_cast<int32>(priority));
#endif
		}

		static void SetAffinityMask(void* threadHandle, uint64 affinityMask)
		{
#ifdef LQ_PLATFORM_WINDOWS
			SetThreadAffinityMask(threadHandle, affinityMask);
#endif
		}

	}

}