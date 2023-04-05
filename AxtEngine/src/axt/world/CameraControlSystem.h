#pragma once

#include <axt/Core.h>

#include "System.h"

#include "axt/ecs/nECS.h"
#include "axt/events/MouseEvent.h"

namespace axt {

	class AXT_API CameraControlSystem : public System {
	public:
		void OnUpdate(float dt, ecs::EntityID cameraID);
		void Resize(float x, float y);
		bool OnEvent(Event& ev);
		bool OnMouseScroll(MouseScrollEvent& ev);
		CameraControlSystem(Ref<ecs::Scene>& scene);
	};

}
