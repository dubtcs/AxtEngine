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

	std::string WindowResizeEvent::ToString() const {
		std::stringstream str;
		str << "WindowResize: (" << width << ", " << height << ")";
		return str.str();
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

	std::string WindowCloseEvent::ToString() const {
		return "WindowClosing";
	}


}
