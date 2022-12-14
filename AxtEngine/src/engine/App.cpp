#include "pch.h"
#include "App.h"

namespace axt {

	App::App() {
		window = std::unique_ptr<AxtWindow>{ AxtWindow::Create() };
	}

	App::~App() {

	}

	void App::Run() {
		while (running) {
			window->Update();

		}
	}

}
