#pragma once

#include "axt/Core.h"
#include <spdlog/spdlog.h>

#include <stdlib.h>

namespace axt {

	class AXT_API Log {
	public:
		Log() {};
		~Log() {};
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLog() { return coreLog; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLog() { return clientLog; }
	private:
		static std::shared_ptr<spdlog::logger> coreLog;
		static std::shared_ptr<spdlog::logger> clientLog;
	};

}

#ifdef AXT_BUILD_DLL
	#define AXT_CORE_ERROR(...) axt::Log::GetCoreLog()->error(__VA_ARGS__)
	#define AXT_CORE_WARN(...) axt::Log::GetCoreLog()->warn(__VA_ARGS__)
	#define AXT_CORE_INFO(...) axt::Log::GetCoreLog()->info(__VA_ARGS__)
	#define AXT_CORE_TRACE(...) axt::Log::GetCoreLog()->trace(__VA_ARGS__)
#else
	#define AXT_CORE_ERROR(...)
	#define AXT_CORE_WARN(...)
	#define AXT_CORE_INFO(...)
	#define AXT_CORE_TRACE(...)
#endif // AXT_BUILD_DLL

#define AXT_ERROR(...) axt::Log::GetClientLog()->error(__VA_ARGS__)
#define AXT_WARN(...) axt::Log::GetClientLog()->warn(__VA_ARGS__)
#define AXT_INFO(...) axt::Log::GetClientLog()->info(__VA_ARGS__)
#define AXT_TRACE(...) axt::Log::GetClientLog()->trace(__VA_ARGS__)

#ifdef AXT_ENABLE_ASSERTS
	#define AXT_ASSERT(condition, msg) if(!condition) { AXT_ERROR("Condition failed {0}: {1}", __FUNCTION__, msg); std::quick_exit(EXIT_FAILURE); }
	#define AXT_CORE_ASSERT(condition, msg) if(!condition) { AXT_CORE_ERROR("Condition failed {0}: {1}", __FUNCTION__, msg); std::quick_exit(EXIT_FAILURE); }
#else
	#define AXT_CORE_ASSERT(condition, msg)
	#define AXT_ASSERT(condition, msg)
#endif
