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

		void PushJob(Job job);
	private:
		void ExecuteJobs();
		Job* GetNextJob();
		void PopJob();

		void Destroy();

		void SetName(const String& name);
		void SetPriority(ThreadPriority priority);
	private:
		ThreadCreateInfo m_CreateInfo;
		std::thread m_Thread;

		std::queue<Job> m_Jobs;
		std::mutex m_Mutex;
		std::condition_variable m_Condition;
		bool m_Destroying = false;
	};

}