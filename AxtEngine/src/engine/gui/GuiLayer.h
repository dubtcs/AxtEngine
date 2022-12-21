#pragma once

#include <engine/Core.h>
#include "engine/Layer.h"

namespace axt {

	class AXT_API GuiLayer : public Layer {
	public:
		GuiLayer();
		~GuiLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& curEvent) override;
	protected:
		float previousTime;
	};

}
