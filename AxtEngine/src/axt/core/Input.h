#pragma once

#include "axt/Core.h"

#include "Keycodes.h"

#include <glm/glm.hpp>

namespace axt 
{

	namespace input 
	{
		bool IsKeyPressed(Keycode keycode);
		bool IsMouseButtonPressed(Keycode keycode);
		glm::vec2 GetMousePosition();
		float GetMouseX();
		float GetMouseY();
	};

}
