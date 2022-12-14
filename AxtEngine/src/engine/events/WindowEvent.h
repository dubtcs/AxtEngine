#pragma once

#include "Event.h"

namespace axt {

	class AXT_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int w, int h) :width{ w }, height{ h } {}
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
		virtual int GetCategoryFlags() const override;
		static const EventType StaticType{ EventType::WindowResize };
		int GetWidth() const { return width; }
		int GetHegith() const { return height; }
	protected:
		int width, height;
	};

	class AXT_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {};
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
		virtual int GetCategoryFlags() const override;
		static const EventType StaticType{ EventType::WindowClose };
	};

}
