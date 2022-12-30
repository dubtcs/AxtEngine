#pragma once

#include "axt/Input.h"

namespace axt {

	class WinInput : public AxtInput {
	protected:
		virtual bool IsKeyPressedI(int keycode) override;
		virtual bool IsMouseButtonPressedI(int keycode) override;
		virtual std::pair<float, float> GetMousePositionI() override;
		virtual float GetMouseXI() override;
		virtual float GetMouseYI() override;
	};

}
