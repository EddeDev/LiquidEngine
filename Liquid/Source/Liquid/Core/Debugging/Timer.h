#pragma once

#include "TimeUnit.h"

#include "Liquid/Core/Logging/LogMacros.h"
#include "Liquid/Core/AssertionMacros.h"

#include <chrono>

namespace Liquid {

	struct Timer
	{
	public:
		Timer(TimeUnit unit = TimeUnit::Seconds)
			: m_Unit(unit)
		{
			Reset();
		}

		void Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		float Elapsed()
		{
			auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start);
			switch (m_Unit)
			{
			case TimeUnit::Seconds:      return static_cast<float>(duration.count()) * 0.001f * 0.001f * 0.001f;
			case TimeUnit::Milliseconds: return static_cast<float>(duration.count()) * 0.001f * 0.001f;
			case TimeUnit::Nanoseconds:  return static_cast<float>(duration.count());
			}
			LQ_VERIFY(false, "Unknown time unit");
			return 0.0f;
		}
	private:
		TimeUnit m_Unit;

		std::chrono::high_resolution_clock::time_point m_Start;
	};

}