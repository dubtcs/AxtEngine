#pragma once

#include "Core.h"
#include "Window.h"

#include "events/Event.h"
#include "events/WindowEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "engine/Layer.h"
#include "engine/LayerStack.h"

namespace axt {

	class AXT_API App {
	public:
		App();
		virtual ~App();
	public:
		void Run();
		bool OnEvent(Event& bindEvent);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool running{ true };
		bool OnWindowClose(WindowCloseEvent& ev);
		std::unique_ptr<AxtWindow> window;
		LayerStack layerstack;
	};

	App* CreateApp();

}