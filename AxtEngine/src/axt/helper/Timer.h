#pragma once

#include "axt/Core.h"

#include <chrono>

namespace axt {

	class AXT_API Timer {
	public:
		Timer(const char* name);
		~Timer();
		void Stop();
	private:
		std::string mName;
		bool mStopped{ false };
		std::chrono::time_point<std::chrono::steady_clock> mStartTime;
	};

}
