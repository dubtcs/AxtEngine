#pragma once

#include <engine/Core.h>
#include "engine/Layer.h"

#include "engine/events/KeyEvent.h"
#include "engine/events/MouseEvent.h"
#include "engine/events/WindowEvent.h"

namespace axt {

	class AXT_API GuiLayer : public Layer {
	public:
		GuiLayer();
		~GuiLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& curEvent) override;
	private:
		bool OnWindowResize(WindowResizeEvent& ev);
		bool OnMouseButtonPressed(MouseButtonPressed& ev);
		bool OnMouseButtonRelease(MouseButtonReleased& ev);
		bool OnMouseMoved(MouseMovedEvent& ev);
		bool OnMouseScroll(MouseScrollEvent& ev);
		bool OnKeyPressed(KeyPressedEvent& ev);
		bool OnKeyReleased(KeyReleasedEvent& ev) { return false; };
		bool OnKeyTyped(KeyTypedEvent& ev);
	private:
		float previousTime;
	};

}
