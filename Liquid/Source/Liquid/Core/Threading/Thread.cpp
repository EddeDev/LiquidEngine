#include "LiquidPCH.h"
#include "Thread.h"

namespace Liquid {

	Thread::Thread(const ThreadCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		m_Thread = std::thread(&Thread::ExecuteJobs, this);

		SetName(createInfo.Name);
		SetPriority(m_CreateInfo.Priority);
	}

	Thread::~Thread()
	{
		Join();
	}

	void Thread::Join()
	{
		if (!m_Thread.joinable())
			return;

		Wait();
		Destroy();

		m_Thread.join();
	}

	void Thread::ExecuteJobs()
	{
		while (true)
		{
			Job* job = GetNextJob();
			if (!job)
				break;

			// Invoke function
			(*job)();

			PopJob();
		}
	}

	Thread::Job* Thread::GetNextJob()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Condition.wait(lock, [this]
		{
			return !m_Jobs.empty() || m_Destroying;
		});

		if (m_Destroying)
			return nullptr;

		return &m_Jobs.front();
	}

	void Thread::PushJob(Job job)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Jobs.push(std::move(job));
		m_Condition.notify_one();
	}

	void Thread::PopJob()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Jobs.pop();
		m_Condition.notify_one();
	}

	void Thread::Wait()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Condition.wait(lock, [this]
		{
			return m_Jobs.empty();
		});
	}

	void Thread::Destroy()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Destroying = true;
		m_Condition.notify_one();
	}

	void Thread::SetName(const String& name)
	{
#ifdef LQ_PLATFORM_WINDOWS
		auto threadHandle = m_Thread.native_handle();
		auto threadName = StringUtils::ToWideString(name);
		SetThreadDescription(threadHandle, threadName.c_str());
#endif
	}

	void Thread::SetPriority(ThreadPriority priority)
	{
#ifdef LQ_PLATFORM_WINDOWS
		auto threadHandle = m_Thread.native_handle();
		SetThreadPriority(threadHandle, static_cast<int32>(priority));
#endif
	}

}