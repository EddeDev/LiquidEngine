#pragma once

#include "Liquid/Core/RefCounting.h"

#include "LogVerbosity.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Liquid {

	class Logger
	{
	public:
		static void Init();
		static void Shutdown();

		static Shared<spdlog::logger>& GetLogger() { return s_Logger; }
	private:
		static Shared<spdlog::logger> s_Logger;
	};

}

#define LQ_LOG_ARGS(verbosity, ...) ::Liquid::Logger::GetLogger()->log(::Liquid::LogUtils::LogVerbosityToSpdlogLevel(::Liquid::LogVerbosity::##verbosity), __VA_ARGS__)
#define LQ_LOG_PREFIX(verbosity, prefix, ...) ::Liquid::Logger::GetLogger()->log(::Liquid::LogUtils::LogVerbosityToSpdlogLevel(::Liquid::LogVerbosity::##verbosity), "{0}: {1}", prefix, fmt::format(__VA_ARGS__))
#define LQ_LOG_CATEGORY(verbosity, categoryName, ...) ::Liquid::Logger::GetLogger()->log(::Liquid::LogUtils::LogVerbosityToSpdlogLevel(::Liquid::LogVerbosity::##verbosity), "[{0}]: {1}", categoryName, fmt::format(__VA_ARGS__))