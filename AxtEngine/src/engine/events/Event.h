#pragma once

#include "../Core.h"
#include <functional>

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
		App, Input, Keyboard, Mouse, MouseButton, MouseScroll
	};

	class AXT_API Event {
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		std::string ToString() const { return GetName(); }
		inline bool IsCategory(EventCategory cat) { return (GetCategoryFlags() & cat); }
	public:
		friend class EventHandler;
		static const EventType Type;
	protected:
		Event() {};
		bool isHandled{ false };
	};

	class AXT_API EventHandler {
	public:
		EventHandler(Event& newEvent) : currentEvent{ newEvent } {};
		~EventHandler() {};
	public:
		void Fire(Event& ev);
	private:
		Event& currentEvent;
	};

}
