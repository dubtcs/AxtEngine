
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
		return Type;
	}

	// KEY RELEASED
	const char* KeyReleasedEvent::GetName() const {
		return "KeyReleasedEvent";
	}
	EventType KeyReleasedEvent::GetEventType() const {
		return Type;
	}


}
