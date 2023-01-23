#pragma once

#include <memory>

#ifdef AXT_PLATFORM_WINDOWS

	#ifdef AXT_DLL
		#ifdef AXT_BUILD_DLL
			#define AXT_API __declspec(dllexport)
		#else
			#define AXT_API __declspec(dllimport)
		#endif
	#else
		#define AXT_API
	#endif

#endif

#define AXT_BIND_EVENT(f) std::bind(&f, this, std::placeholders::_1)

namespace axt {

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Unique = std::unique_ptr<T>;

}
