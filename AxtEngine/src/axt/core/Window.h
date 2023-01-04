#pragma once

#include "axt/Core.h"
#include <pch.h>
#include "axt/events/Event.h"

#include "axt/render/RenderContext.h"

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
		virtual void Update() = 0;
		virtual void SetVsync(bool toggle) = 0;
		virtual void SetEventCallback(std::function<bool(Event&)>) {};
		virtual bool IsVsync() const { return false; };
		virtual unsigned int GetWidth() const { return 1; };
		virtual unsigned int GetHeight() const { return 1; };
		virtual void* GetNativeWindow() const = 0;
	protected:
		RenderContext* context{ nullptr };
		AxtWindow() {};
	};

}
