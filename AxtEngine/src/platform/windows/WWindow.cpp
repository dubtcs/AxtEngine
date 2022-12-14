
#include <pch.h>
#include "WWindow.h"
#include "engine/Log.h"

#define uint unsigned int

namespace axt {

	static bool axtGlfwInit{ false };

	AxtWindow* AxtWindow::Create(const AxtWindowConfig& config) {
		return new WWindow(config);
	}

	WWindow::WWindow(const AxtWindowConfig& config) : data{ config } {
		Init();
	}

	WWindow::~WWindow() {
		Shutdown();
	}

	void WWindow::Init() {
		//AXT_CORE_INFO("Creating window {0} ({1}, {2})", data.title, data.width, data.height);
		if (!axtGlfwInit) {
			int suc{ glfwInit() };
			if (suc == 1) {
				axtGlfwInit = true;
			}
		}
		window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);
		if (window != nullptr) {
			glfwMakeContextCurrent(window);
			glfwSetWindowUserPointer(window, &data);
			SetVsync(true);
		}
			//AXT_CORE_ERROR("Could not create window");
	}

	void WWindow::Shutdown() {
		glfwDestroyWindow(window);
	}

	void WWindow::Update() {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WWindow::SetVsync(bool toggle) {
		glfwSwapInterval((toggle) ? 1 : 0);
		data.vsync = toggle;
	}

	bool WWindow::IsVsync() const {
		return data.vsync;
	}

	uint WWindow::GetHeight() const {
		return data.height;
	}

	uint WWindow::GetWidth() const {
		return data.width;
	}

}
