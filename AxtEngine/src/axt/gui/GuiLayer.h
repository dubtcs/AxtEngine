#pragma once

#include "axt/Core.h"
#include "axt/core/Layer.h"

#include "axt/events/KeyEvent.h"
#include "axt/events/MouseEvent.h"
#include "axt/events/WindowEvent.h"

namespace axt {

	class AXT_API GuiLayer : public Layer {
	public:
		GuiLayer();
		~GuiLayer();
		void OnAttach() override;
		void OnDetach() override;
		//void OnUpdate() override;
		void OnImGuiRender() override;
		void Begin();
		void End();
	private:
		float previousTime;
	};

}
