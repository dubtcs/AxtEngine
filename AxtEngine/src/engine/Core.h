#pragma once

#ifdef AXT_PLATFORM_WINDOWS
#ifdef AXT_BUILD_DLL
#define AXT_API __declspec(dllexport)
#else
#define AXT_API __declspec(dllimport)
#endif
#define AXT namespace axt
#endif

#include <string>
