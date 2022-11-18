#pragma once

#include "Liquid/Core/PrimitiveTypes.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

#include <string>

namespace Liquid {

	enum class LogVerbosity : uint8
	{
		None = 0,
		
		Trace,
		Info,
		Warning,
		Error,
		Critical
	};

	namespace LogUtils {

		static const char* LogVerbosityToString(LogVerbosity verbosity)
		{
			switch (verbosity)
			{
			case LogVerbosity::None:     return "None";
			case LogVerbosity::Trace:    return "Trace";
			case LogVerbosity::Info:     return "Info";
			case LogVerbosity::Warning:  return "Warning";
			case LogVerbosity::Error:    return "Error";
			case LogVerbosity::Critical: return "Critical";
			}
			return "UnknownVerbosity";
		}

		static LogVerbosity LogVerbosityFromString(const std::string& verbosityString)
		{
			if (verbosityString == "Trace")
				return LogVerbosity::Trace;
			else if (verbosityString == "Info")
				return LogVerbosity::Info;
			else if (verbosityString == "Warning")
				return LogVerbosity::Warning;
			else if (verbosityString == "Error")
				return LogVerbosity::Error;
			else if (verbosityString == "Critical")
				return LogVerbosity::Critical;
			return LogVerbosity::None;
		}

		static spdlog::level::level_enum LogVerbosityToSpdlogLevel(LogVerbosity verbosity)
		{
			switch (verbosity)
			{
			case Liquid::LogVerbosity::Trace:    return spdlog::level::trace;
			case Liquid::LogVerbosity::Info:     return spdlog::level::info;
			case Liquid::LogVerbosity::Warning:  return spdlog::level::warn;
			case Liquid::LogVerbosity::Error:    return spdlog::level::err;
			case Liquid::LogVerbosity::Critical: return spdlog::level::critical;
			}
			return spdlog::level::off;
		}

	}

}