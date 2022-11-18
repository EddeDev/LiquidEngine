#pragma once

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

		static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

}

#define LQ_LOG_ARGS(verbosity, ...) \
{ \
	auto level = ::Liquid::LogUtils::LogVerbosityToSpdlogLevel(verbosity); \
	::Liquid::Logger::GetLogger()->log(level, __VA_ARGS__); \
}

#define LQ_LOG_PREFIX(verbosity, prefix, ...) \
{ \
	auto level = ::Liquid::LogUtils::LogVerbosityToSpdlogLevel(verbosity); \
	::Liquid::Logger::GetLogger()->log(level, "{0}: {1}", prefix, fmt::format(__VA_ARGS__)); \
}

#define LQ_LOG_CATEGORY(verbosity, categoryName, ...) \
{ \
	auto level = ::Liquid::LogUtils::LogVerbosityToSpdlogLevel(verbosity); \
	::Liquid::Logger::GetLogger()->log(level, "[{0}]: {1}", categoryName, fmt::format(__VA_ARGS__)); \
}
