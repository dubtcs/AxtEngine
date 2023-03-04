#pragma once

#include "axt/Core.h"

namespace axt {

	namespace input {
		bool IsKeyPressed(int keycode);
		bool IsMouseButtonPressed(int keycode);
		std::pair<float, float> GetMousePosition();
		float GetMouseX();
		float GetMouseY();
	};

}
