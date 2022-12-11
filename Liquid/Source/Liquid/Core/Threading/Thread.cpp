#include "LiquidPCH.h"
#include "Thread.h"

namespace Liquid {

	Thread::Thread(const ThreadCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		m_Thread = std::thread(&Thread::ExecuteJobs, this);

		ThreadUtils::SetName(m_Thread.native_handle(), createInfo.Name);
		ThreadUtils::SetPriority(m_Thread.native_handle(), m_CreateInfo.Priority);
		// ThreadUtils::SetAffinityMask(m_Thread.native_handle(), 8);
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

	void Thread::PushJob(StringView description, Job job)
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

}