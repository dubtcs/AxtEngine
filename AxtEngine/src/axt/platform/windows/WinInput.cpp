
#include <pch.h>

// this will only compile on windows
#include "axt/core/Input.h"

#include <GLFW/glfw3.h>
#include "axt/core/App.h"

#ifdef AXT_PLATFORM_WINDOWS
namespace axt {
	namespace input {

		bool IsKeyPressed(Keycode keycode) {
			const auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
			const int state{ glfwGetKey(window, keycode) };
			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}

		bool IsMouseButtonPressed(Keycode keycode) {
			const auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
			const int state{ glfwGetMouseButton(window, keycode) };
			return state == GLFW_PRESS;
		}

		glm::vec2 GetMousePosition() {
			const auto window = static_cast<GLFWwindow*>(App::GetApp().GetWindow().GetNativeWindow());
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			return { static_cast<float>(x), static_cast<float>(y) };
		}

		float GetMouseX() {
			glm::vec2 pos{ GetMousePosition() };
			return pos.x;
		}

		float GetMouseY() {
			glm::vec2 pos{ GetMousePosition() };
			return pos.y;
		}

	}	
}
#endif
