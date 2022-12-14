#include <pch.h>

#include "WindowEvent.h"

namespace axt {

	// Resize
	EventType WindowResizeEvent::GetEventType() const {
		return StaticType;
	}

	const char* WindowResizeEvent::GetName() const {
		return "WindowResizeEvent";
	}

	int WindowResizeEvent::GetCategoryFlags() const {
		return (EventCategory::Application);
	}

	// Close
	EventType WindowCloseEvent::GetEventType() const {
		return StaticType;
	}

	const char* WindowCloseEvent::GetName() const {
		return "WindowCloseEvent";
	}

	int WindowCloseEvent::GetCategoryFlags() const {
		return (EventCategory::Application);
	}

}
