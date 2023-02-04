#pragma once

// static profiler

#include "axt/Core.h"

#include <fstream>
#include <chrono>

namespace axt {

	namespace profiling {

		struct AXT_API ProfileData {
			const char* name;
			long long mStart, mEnd;
		};

		class AXT_API Profile {
		public:
			Profile(const char* name) : mName{ name }, mStopped{ false } {}
			~Profile();
			void Stop();
		private:
			const char* mName;
			std::chrono::high_resolution_clock::time_point mStartTime{ std::chrono::high_resolution_clock::now() };
			bool mStopped;
		};

		class AXT_API ProfileSession {
		public:
			static void Begin(const char* sessionName, const char* filepath = "ProfilerResultsAXT.json");
			static void End();
			static void Header();
			static void Footer();
			static void WriteData(const ProfileData& data);
		private:
			static ProfileSession mInstance;
			std::ofstream mFileStream;
			int mProfileCount{ 0 };
		};

	}

}

#define AXT_PROFILING_ENABLED 1
#ifdef AXT_PROFILING_ENABLED
	#define AXT_PROFILE_NEW_SESSION(name, filepath) axt::profiling::ProfileSession::Begin(name, filepath)
	#define AXT_PROFILE_END_SESSION() axt::profiling::ProfileSession::End()
	#define AXT_PROFILE_SCOPE(name) axt::profiling::Profile profile##__LINE__{name}
	#define AXT_PROFILE_FUNCTION() AXT_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define AXT_PROFILE_NEW_SESSION(name, filepath)
	#define AXT_PROFILE_END_SESSION()
	#define AXT_PROFILE_SCOPE(name)
	#define AXT_PROFILE_FUNCTION()
#endif
