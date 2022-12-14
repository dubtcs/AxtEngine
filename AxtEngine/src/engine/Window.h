#pragma once

#include "Core.h"
#include <pch.h>

namespace axt {

	struct AXT_API AxtWindowConfig {
		unsigned int width, height;
		std::string title;
		AxtWindowConfig(std::string title = "AxtWindow", unsigned int w = 1920, unsigned int h = 1080) : title{title}, width{w}, height{h} {};
	};

	class AXT_API AxtWindow {
	public:
		// Platform agnostic window creation
		static AxtWindow* Create(const AxtWindowConfig& config = AxtWindowConfig{});
	public:
		virtual void Update() {};
		virtual void SetVsync(bool toggle) {};
		virtual bool IsVsync() const { return false; };
		virtual unsigned int GetWidth() const { return 1; };
		virtual unsigned int GetHeight() const { return 1; };
	protected:
		AxtWindow() {};
	};

}
