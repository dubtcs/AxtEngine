#pragma once

#include "../Core.h"

namespace axt {

	enum class EventType {
		NONE = 0,
		WindowClose, WindowResize, WindowFocusGain, WindowFocusLost, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScroll
	};

	enum EventCategory {
		None,
		Application, Input, Keyboard, Mouse, MouseButton, MouseScroll
	};

	class AXT_API Event {
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }
		inline bool IsCategory(EventCategory cat) { return (GetCategoryFlags() & cat); }
		bool Handled() { return isHandled; }
	public:
		friend class EventHandler;
		static const EventType StaticType;
	protected:
		Event() {};
		bool isHandled{ false };
	};

	class AXT_API EventHandler {
	public:
		EventHandler(axt::Event& newEvent) : currentEvent{ newEvent } {};
		~EventHandler() {};
	public:
		template <typename T>
		bool Fire(std::function<bool(T&)> eventFunction) {
			if (currentEvent.GetEventType() == T::StaticType) {
				currentEvent.isHandled = eventFunction(*(T*)&currentEvent); // cast to type T  --> Mem address of event into a T pointer, then dereference it
				return true;
			}
			return false;
		}
	private:
		axt::Event& currentEvent;
	};

}
