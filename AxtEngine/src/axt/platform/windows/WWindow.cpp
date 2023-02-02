
#include <pch.h>
#include <assert.h>
#include "WWindow.h"
#include "axt/core/Log.h"
#include "axt/platform/OpenGL/GLContext.h"

#include <glad/glad.h>

// Events
#include "axt/events/WindowEvent.h"
#include "axt/events/KeyEvent.h"
#include "axt/events/MouseEvent.h"

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

	void WWindow::GlfwErrorCallback(int er, const char* msg) {
		AXT_ERROR("glfw error ({0}): {1}", er, msg);
	}

	void WWindow::Init() {

		AXT_CORE_INFO("Creating window {0} ({1}, {2})", data.title, data.width, data.height);
		if (!axtGlfwInit) {
			int suc{ glfwInit() };
			if (suc == 1) {
				axtGlfwInit = true;
			}
		}
		window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);
		if (window != nullptr) {

			context = new OpenGLContext{ window };
			context->Init();

			glfwSetWindowUserPointer(window, &data);
			SetVsync(true);
		} else
			AXT_CORE_ERROR("Could not create window");

		glfwSetWindowSizeCallback(window, [](GLFWwindow* win, int w, int h) {
				WindowData& winData = *(WindowData*)glfwGetWindowUserPointer(win);
				winData.width = w;
				winData.height = h;
				WindowResizeEvent resizeEvent{ w, h };
				winData.callback(resizeEvent);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* win) {
			WindowData& winData = *(WindowData*)glfwGetWindowUserPointer(win);
			WindowCloseEvent close{};
			winData.callback(close);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int code, int action, int mods) {
			WindowData& winData = *(WindowData*)glfwGetWindowUserPointer(win);
			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent ev{ key, 0 };
					winData.callback(ev);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent ev{ key };
					winData.callback(ev);
					break;
				}
				case GLFW_REPEAT:
					break;
			}
		});

		glfwSetCharCallback(window, [](GLFWwindow* win, uint keycode) {
			WindowData& winData = *(WindowData*)glfwGetWindowUserPointer(win);
			KeyTypedEvent ev{ (int)keycode };
			winData.callback(ev);
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int key, int action, int mods) {
			WindowData& winData = *(WindowData*)glfwGetWindowUserPointer(win);
			switch (action) {
			case GLFW_PRESS: {
				MouseButtonPressed ev{ key };
				winData.callback(ev);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleased ev{ key };
				winData.callback(ev);
				break;
			}
			}
		});

		glfwSetScrollCallback(window, [](GLFWwindow* win, double x, double y) {
			WindowData& winData = *(WindowData*)glfwGetWindowUserPointer(win);
			MouseScrollEvent ev{ x, y };
			winData.callback(ev);
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
			WindowData& winData = *(WindowData*)glfwGetWindowUserPointer(win); \
			MouseMovedEvent ev{ x, y };
			winData.callback(ev);
		});

	}

	void WWindow::Shutdown() {
		glfwDestroyWindow(window);
	}

	void WWindow::Update() {
		glfwPollEvents();
		context->SwapBuffers();
	}

	void WWindow::SetEventCallback(std::function<bool(Event&)> bindFunction) {
		data.callback = bindFunction;
		return;
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
