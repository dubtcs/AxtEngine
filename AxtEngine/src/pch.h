#pragma once

// disable min and max macros
#define NOMINMAX

#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>
#include <functional>
#include <memory>
#include <algorithm>

#include <string>
#include <vector>
#include <array>

#include "axt/core/Log.h"

#include "axt/helper/Profiler.h"
