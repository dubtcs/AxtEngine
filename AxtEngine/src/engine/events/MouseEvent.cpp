#include <pch.h>
#include "MouseEvent.h"

namespace axt {

	// BASE
	int MouseButtonEvent::GetCategoryFlags() const {
		return (EventCategory::MouseButton | EventCategory::Input);
	}

	// PRESSED
	const char* MouseButtonPressed::GetName() const {
		return "MouseButtonPressed";
	}

	EventType MouseButtonPressed::GetEventType() const {
		return StaticType;
	}

	// RELEASED
	const char* MouseButtonReleased::GetName() const {
		return "MouseButtonReleased";
	}

	EventType MouseButtonReleased::GetEventType() const {
		return StaticType;
	}

	// MOUSE MOVEMENT
	int MouseMovedEvent::GetCategoryFlags() const {
		return (EventCategory::Mouse | EventCategory::Input);
	}

	const char* MouseMovedEvent::GetName() const {
		return "MouseMovedEvent";
	}

	EventType MouseMovedEvent::GetEventType() const {
		return EventType::MouseMoved;
	}

	// MOUSE SCROLL
	int MouseScrollEvent::GetCategoryFlags() const {
		return (EventCategory::MouseScroll | EventCategory::Input);
	}

	const char* MouseScrollEvent::GetName() const {
		return "MouseScrollEvent";
	}

	EventType MouseScrollEvent::GetEventType() const {
		return EventType::MouseScroll;
	}

}
