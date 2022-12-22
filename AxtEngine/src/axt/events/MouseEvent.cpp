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

	std::string MouseButtonPressed::ToString() const {
		std::stringstream str;
		str << "MouseButtonPressed: " << button;
		return str.str();
	}

	// RELEASED
	const char* MouseButtonReleased::GetName() const {
		return "MouseButtonReleased";
	}

	EventType MouseButtonReleased::GetEventType() const {
		return StaticType;
	}
	std::string MouseButtonReleased::ToString() const {
		std::stringstream str;
		str << "MouseButtonReleased: " << button;
		return str.str();
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

	std::string MouseMovedEvent::ToString() const {
		std::stringstream str;
		str << "MouseMoved: (" << x << ", " << y << ")";
		return str.str();
	}

	double MouseMovedEvent::GetX() {
		return x;
	}
	double MouseMovedEvent::GetY() {
		return y;
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

	std::string MouseScrollEvent::ToString() const {
		std::stringstream str;
		str << "MouseScroll: (" << x << ", " << y << ")";
		return str.str();
	}

	double MouseScrollEvent::GetX() {
		return x;
	}
	double MouseScrollEvent::GetY() {
		return y;
	}

}
