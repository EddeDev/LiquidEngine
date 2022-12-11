#pragma once

#include "TimeUnit.h"

#include "Liquid/Core/Logging/LogMacros.h"
#include "Liquid/Core/String.h"

#include <chrono>

namespace Liquid {

	struct ScopedTimer
	{
	public:
		ScopedTimer(StringView name, TimeUnit unit = TimeUnit::Seconds)
			: m_Name(name), m_Unit(unit)
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		~ScopedTimer()
		{
			m_Stop = std::chrono::high_resolution_clock::now();

			switch (m_Unit)
			{
			case TimeUnit::Seconds:
			{
				auto duration = std::chrono::duration_cast<std::chrono::seconds>(m_Stop - m_Start);
				LQ_INFO_ARGS("[TIMER]: {0}: {1} seconds", m_Name, duration.count());
				break;
			}
			case TimeUnit::Milliseconds:
			{
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_Stop - m_Start);
				LQ_INFO_ARGS("[TIMER]: {0}: {1}ms", m_Name, duration.count());
				break;
			}
			case TimeUnit::Nanoseconds:
			{
				auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(m_Stop - m_Start);
				LQ_INFO_ARGS("[TIMER]: {0}: {1}ns", m_Name, duration.count());
				break;
			}
			}
		}
	private:
		StringView m_Name;
		TimeUnit m_Unit;

		std::chrono::high_resolution_clock::time_point m_Start;
		std::chrono::high_resolution_clock::time_point m_Stop;
	};

}