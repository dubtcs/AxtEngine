#pragma once

#include "axt/Core.h"
#include "Window.h"

#include "axt/events/Event.h"
#include "axt/events/WindowEvent.h"
#include "axt/events/KeyEvent.h"
#include "axt/events/MouseEvent.h"

#include "axt/core/Layer.h"
#include "axt/core/LayerStack.h"

#include "axt/gui/GuiLayer.h"

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
		static App& GetApp() { return *instance; }
		AxtWindow& GetWindow() { return *window; }
	private:
		bool running{ true };
		bool OnWindowClose(WindowCloseEvent& ev);
		std::unique_ptr<AxtWindow> window;
		GuiLayer* guilayer;
		LayerStack layerstack;
		static App* instance;
	};

	App* CreateApp();

}