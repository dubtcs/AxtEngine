#pragma once

#include "Event.h"

namespace axt {

	class AXT_API KeyEvent : public axt::Event {
	public:
		virtual int GetCategoryFlags() const override;
		inline int GetKeycode() { return keycode; }
	protected:
		KeyEvent(int keycode) : keycode{ keycode } {}
		int keycode;
	};

	class AXT_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int amount) : KeyEvent(keycode), amountPressed{ amount } {}
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
		static const EventType StaticType{ EventType::KeyPressed };
		std::string ToString() const override;
	private:
		int amountPressed;
	};

	class AXT_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
		static const EventType StaticType{ EventType::KeyReleased };
		std::string ToString() const override;
	};

	class AXT_API KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keycode) : KeyEvent(keycode) {}
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
		static const EventType StaticType{ EventType::KeyTyped };
		std::string ToString() const override;
	};

}
