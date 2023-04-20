#pragma once

#include <memory>

// use without premake flags
#ifdef _WIN32
	#ifdef _WIN64
		#define AXT_PLATFORM_WINDOWS
	#else
		#error "Only x64 is supported"
	#endif
#endif

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

//#define AXT_BIND(f) [this](auto&&... ar) -> declytype(auto) { return this->f(std::forward<decltype(ar)>(ar)...) }

namespace axt {

	inline const char* AXT_FILE_EXTENSION{ ".axtp" };

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... A>
	constexpr Ref<T> NewRef(A&&... a) {
		return std::make_shared<T>(std::forward<A>(a)...);
	}

	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T, typename... A>
	constexpr Unique<T> NewUnique(A&&... a) {
		return std::make_unique<T>(std::forward<A>(a)...);
	}

}
