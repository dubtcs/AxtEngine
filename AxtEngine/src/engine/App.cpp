
#include "pch.h"
#include "App.h"
#include "Log.h"

#include <glad/glad.h>

namespace axt {

	App::App() {
		window = std::unique_ptr<AxtWindow>{ AxtWindow::Create() };
		window->SetEventCallback(std::bind(&App::OnEvent, this, std::placeholders::_1));
	}

	App::~App() {

	}

	void App::Run() {
		while (running) {
			glClearColor(0.25f, 0.25f, 0.25f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);
			for (Layer* curLayer : layerstack) {
				curLayer->OnUpdate();
			}
			window->Update();
		}
	}

	bool App::OnEvent(Event& bindEvent) {
		EventHandler handler{ bindEvent };
		handler.Fire<WindowCloseEvent>(std::bind(&App::OnWindowClose, this, std::placeholders::_1));

		AXT_TRACE("{0}", bindEvent.ToString());

		// back to front for events
		for (std::vector<Layer*>::iterator iter{ layerstack.end() }; iter != layerstack.begin();) {
			(*--iter)->OnEvent(bindEvent);
			if (bindEvent.Handled()) {
				break;
			}
		}

		return true;
	}

	void App::PushLayer(Layer* layer) {
		layerstack.PushLayer(layer);
	}

	void App::PushOverlay(Layer* overlay) {
		layerstack.PushOverlay(overlay);
	}

	bool App::OnWindowClose(WindowCloseEvent& ev) {
		running = false;
		return true;
	}

}
