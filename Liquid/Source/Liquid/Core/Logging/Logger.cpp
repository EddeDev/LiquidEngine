#include "LiquidPCH.h"
#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <filesystem>

namespace Liquid {

	Shared<spdlog::logger> Logger::s_Logger;

	void Logger::Init()
	{
		std::filesystem::path outputDirectory = "Logs";
		if (!std::filesystem::exists(outputDirectory))
			std::filesystem::create_directories(outputDirectory);

		std::filesystem::path filepath = outputDirectory / "Latest.log";

		std::vector<spdlog::sink_ptr> sinks =
		{
			CreateShared<spdlog::sinks::stdout_color_sink_mt>(),
			CreateShared<spdlog::sinks::basic_file_sink_mt>(filepath.string(), true)
		};

		sinks[0]->set_pattern("%^[%T] %n: %v%$");
		sinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_Logger = std::make_shared<spdlog::logger>("Liquid", sinks.begin(), sinks.end());
		s_Logger->set_level(spdlog::level::trace);
		s_Logger->flush_on(spdlog::level::trace);
	}

	void Logger::Shutdown()
	{
		s_Logger.reset();
		spdlog::shutdown();
	}

}