#pragma once

#include "axt/Core.h"

namespace axt {

	class AXT_API AxtInput {
	public:
		static bool IsKeyPressed(int keycode) { return instance->IsKeyPressedI(keycode); };
		static bool IsMouseButtonPressed(int keycode) { return instance->IsMouseButtonPressedI(keycode); }
		static std::pair<float, float> GetMousePosition() { return instance->GetMousePositionI(); }
		static float GetMouseX() { return instance->GetMouseXI(); }
		static float GetMouseY() { return instance->GetMouseYI(); }
	protected:
		virtual bool IsKeyPressedI(int keycode) = 0;
		virtual bool IsMouseButtonPressedI(int keycode) = 0;
		virtual std::pair<float, float> GetMousePositionI() = 0;
		virtual float GetMouseXI() = 0;
		virtual float GetMouseYI() = 0;
	private:
		static AxtInput* instance;
	};

}
