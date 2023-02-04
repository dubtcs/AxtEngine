#include <pch.h>

#include "Timer.h"

namespace axt {

	Timer::Timer(const char* name) : mName{ name } {
		mStartTime = std::chrono::high_resolution_clock::now();
	}

	Timer::~Timer() {
		if (!mStopped) {
			Stop();
		}
	}

	void Timer::Stop() {
		mStopped = true;
		auto lEndTime{ std::chrono::high_resolution_clock::now() };
		long long start{ std::chrono::time_point_cast<std::chrono::microseconds>(mStartTime).time_since_epoch().count() };
		long long end{ std::chrono::time_point_cast<std::chrono::microseconds>(lEndTime).time_since_epoch().count() };
		AXT_TRACE("Timer {0}: {1}ms", mName, (end - start) * 0.001f);
	}

}