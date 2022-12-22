#pragma once

#include "Event.h"

namespace axt {

	class MouseButtonEvent : public axt::Event {
	public:
		virtual int GetCategoryFlags() const override;
		inline int GetButton() { return button; }
	protected:
		MouseButtonEvent(int button) : button{ button } {};
	protected:
		int button;
	};

	class MouseButtonPressed : public MouseButtonEvent {
	public:
		MouseButtonPressed(int button) : MouseButtonEvent(button) {};
		static const EventType StaticType{ EventType::MouseButtonPressed };
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
		std::string ToString() const override;
	};

	class MouseButtonReleased : public MouseButtonEvent {
	public:
		MouseButtonReleased(int button) : MouseButtonEvent(button) {};
		static const EventType StaticType{ EventType::MouseButtonReleased };
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
		std::string ToString() const override;
	};

	class MouseMovedEvent : public Event {
	public:
		virtual int GetCategoryFlags() const override;
		MouseMovedEvent(double x, double y) : x{ x }, y{ y } {};
		static const EventType StaticType{ EventType::MouseMoved };
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
		std::string ToString() const override;
		double GetX();
		double GetY();
	private:
		double x, y;
	};

	class MouseScrollEvent : public Event {
	public:
		virtual int GetCategoryFlags() const override;
		MouseScrollEvent(double x, double y) : x{ x }, y{ y } {};
		static const EventType StaticType{ EventType::MouseScroll };
		virtual const char* GetName() const override;
		virtual EventType GetEventType() const override;
		std::string ToString() const override;
		double GetX();
		double GetY();
	private:
		double x, y;
	};

}
