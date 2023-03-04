
#include <pch.h>

// this will only compile on windows
#include "axt/core/Input.h"

#include <GLFW/glfw3.h>
#include "axt/core/App.h"

#ifdef AXT_PLATFORM_WINDOWS
namespace axt {
	namespace input {

		bool IsKeyPressed(int keycode) {
			const auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
			const int state{ glfwGetKey(window, keycode) };
			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}

		bool IsMouseButtonPressed(int keycode) {
			const auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
			const int state{ glfwGetMouseButton(window, keycode) };
			return state == GLFW_PRESS;
		}

		std::pair<float, float> GetMousePosition() {
			const auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			return { (float)x, (float)y };
		}

		float GetMouseX() {
			std::pair<float, float> pos{ GetMousePosition() };
			return pos.first;
		}

		float GetMouseY() {
			std::pair<float, float> pos{ GetMousePosition() };
			return pos.second;
		}

	}	
}
#endif
