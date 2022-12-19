#include <pch.h>
#include "KeyEvent.h"

namespace axt {

	// BASE
	int KeyEvent::GetCategoryFlags() const {
		return (EventCategory::Keyboard | EventCategory::Input);
	}

	// KEY PRESSED
	const char* KeyPressedEvent::GetName() const {
		return "KeyPressedEvent";
	}
	EventType KeyPressedEvent::GetEventType() const {
		return StaticType;
	}
	std::string KeyPressedEvent::ToString() const {
		std::stringstream str;
		str << "KeyPressed: " << keycode;
		return str.str();
	}


	// KEY RELEASED
	const char* KeyReleasedEvent::GetName() const {
		return "KeyReleasedEvent";
	}
	EventType KeyReleasedEvent::GetEventType() const {
		return StaticType;
	}
	std::string KeyReleasedEvent::ToString() const {
		std::stringstream str;
		str << "KeyReleased: " << keycode;
		return str.str();
	}



}
