#pragma once

#include "Event.h"

namespace axt {

	class MouseButtonEvent : public Event {
	public:
		virtual int GetCategoryFlags() const override;
		inline int GetButton() { return button; }
	protected:
		MouseButtonEvent(int button) : button{ button } {};
	private:
		int button;
	};

	class MouseButtonPressed : public MouseButtonEvent {
	public:
		MouseButtonPressed(int button) : MouseButtonEvent(button) {};
		static const EventType Type{ EventType::MouseButtonPressed };
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
	};

	class MouseButtonReleased : public MouseButtonEvent {
		MouseButtonReleased(int button) : MouseButtonEvent(button) {};
		static const EventType Type{ EventType::MouseButtonReleased };
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
	};

	class MouseMovedEvent : public Event {
	public:
		virtual int GetCategoryFlags() const override;
		MouseMovedEvent(int x, int y) : x{ x }, y{ y } {};
		static const EventType Type{ EventType::MouseMoved };
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
	private:
		int x, y;
	};

	class MouseScrollEvent : public Event {
	public:
		virtual int GetCategoryFlags() const override;
		MouseScrollEvent(int x, int y) : x{ x }, y{ y } {};
		static const EventType Type{ EventType::MouseScroll };
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
	private:
		int x, y;
	};

}
