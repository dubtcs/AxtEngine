#include <pch.h>
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace axt {

	std::shared_ptr<spdlog::logger> Log::coreLog;
	std::shared_ptr<spdlog::logger> Log::clientLog;
	
	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		coreLog = spdlog::stdout_color_mt("Engine");
		coreLog->set_level(spdlog::level::trace);
		clientLog = spdlog::stdout_color_mt("Client");
		clientLog->set_level(spdlog::level::trace);
	}

}
