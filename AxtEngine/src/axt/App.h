#pragma once

#include "Core.h"
#include "Window.h"

#include "events/Event.h"
#include "events/WindowEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "axt/Layer.h"
#include "axt/LayerStack.h"

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