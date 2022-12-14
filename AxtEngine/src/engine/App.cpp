
#include "pch.h"
#include "App.h"
#include "Log.h"

namespace axt {

	App::App() {
		window = std::unique_ptr<AxtWindow>{ AxtWindow::Create() };
		window->SetEventCallback(std::bind(&App::OnEvent, this, std::placeholders::_1));
	}

	App::~App() {

	}

	void App::Run() {
		while (running) {
			window->Update();

		}
	}

	bool App::OnEvent(Event& bindEvent) {
		EventHandler handler{ bindEvent };
		handler.Fire<WindowCloseEvent>(std::bind(&App::OnWindowClose, this, std::placeholders::_1));

		AXT_TRACE("{0}", bindEvent.ToString());
		return true;
	}

	bool App::OnWindowClose(WindowCloseEvent& ev) {
		running = false;
		return true;
	}

}
