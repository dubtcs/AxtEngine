#include <pch.h>

#include "Profiler.h"

namespace axt {

	namespace profiling {

		ProfileSession ProfileSession::mInstance{};

		void ProfileSession::Begin(const char* name, const char* filepath) {
			mInstance.mFileStream.open(filepath);
			Header();
		}

		void ProfileSession::Header() {
			mInstance.mFileStream << "{\"otherData\": {},\n\"traceEvents\": [\n";
			mInstance.mFileStream.flush();
		}

		void ProfileSession::End() {
			Footer();
			mInstance.mFileStream.close();
		}

		void ProfileSession::Footer() {
			mInstance.mFileStream << "]}";
			mInstance.mFileStream.flush();
			mInstance.mProfileCount = 0;
		}

		void ProfileSession::WriteData(const ProfileData& result) {
			std::string fTitle{ result.name };
			std::replace(fTitle.begin(), fTitle.end(), '"', '\"');

			if (mInstance.mProfileCount++ > 0) {
				mInstance.mFileStream << ",";
			}

			//trying to make it a little readable when opened with a text editor lol
			mInstance.mFileStream << "{\n\t\"cat\":\"function\",";
			mInstance.mFileStream << "\n\t\"dur\":" << result.mEnd - result.mStart << ",\n\t";
			mInstance.mFileStream << "\"name\":\""<< fTitle << "\",\n\t";
			mInstance.mFileStream << "\"ph\":\"X\",\n\t";
			mInstance.mFileStream << "\"pid\":0,\n\t";
			mInstance.mFileStream << "\"tid\":0,\n\t";
			mInstance.mFileStream << "\"ts\":" << result.mStart << "\n";
			mInstance.mFileStream << "}\n";

			mInstance.mFileStream.flush();
		}

		Profile::~Profile() {
			if (!mStopped) {
				Stop();
			}
		}

		void Profile::Stop() {
			mStopped = true;
			std::chrono::high_resolution_clock::time_point fCurrentTime{ std::chrono::high_resolution_clock::now() };
			long long start{ std::chrono::time_point_cast<std::chrono::microseconds>(mStartTime).time_since_epoch().count() };
			long long end{ std::chrono::time_point_cast<std::chrono::microseconds>(fCurrentTime).time_since_epoch().count() };
			ProfileSession::WriteData({ mName, start, end });
		}

	}

}
