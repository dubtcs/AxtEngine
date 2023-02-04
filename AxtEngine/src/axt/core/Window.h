#pragma once

#include "axt/Core.h"
#include <pch.h>
#include "axt/events/Event.h"

#include "axt/render/RenderContext.h"

constexpr unsigned int WIDTH{ 1920 };
constexpr unsigned int HEIGHT{ 1080 };

namespace axt {

	struct AXT_API AxtWindowConfig {
		unsigned int width, height;
		std::string title;
		AxtWindowConfig(std::string title = "AxtWindow", unsigned int w = WIDTH, unsigned int h = HEIGHT) : title{title}, width{w}, height{h} {};
	};

	class AXT_API AxtWindow {
	public:
		// Platform agnostic window creation
		static Unique<AxtWindow> Create(const AxtWindowConfig& config = AxtWindowConfig{});
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
