
#include <pch.h>
#include "WinInput.h"

#include <GLFW/glfw3.h>
#include "axt/core/App.h"

namespace axt {

	AxtInput* WinInput::instance{ new WinInput{} };

	bool WinInput::IsKeyPressedI(int keycode) {
		const auto window = static_cast<GLFWwindow*>( App::GetApp().GetWindow().GetNativeWindow() );
		const int state{ glfwGetKey(window, keycode) };
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WinInput::IsMouseButtonPressedI(int keycode) {
		const auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
		const int state{ glfwGetMouseButton(window, keycode) };
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WinInput::GetMousePositionI() {
		const auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	float WinInput::GetMouseXI() {
		std::pair<float, float> pos{ GetMousePositionI() };
		return pos.first;
	}

	float WinInput::GetMouseYI() {
		std::pair<float, float> pos{ GetMousePositionI() };
		return pos.second;
	}

	

}
