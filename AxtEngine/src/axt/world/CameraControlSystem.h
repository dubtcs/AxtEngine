#pragma once

#include <axt/Core.h>

#include "System.h"
#include "axt/events/Event.h"
#include "axt/events/WindowEvent.h"
#include "axt/events/MouseEvent.h"

#include <necs/include.h>

namespace axt
{

	class AXT_API CameraControlSystem : protected System
	{
	public:
		CameraControlSystem(Ref<necs::Scene>& scene);
		void OnUpdate(float dt, const necs::Entity& cameraEntity);
		void OnResize(float x, float y);
		bool OnEvent(Event& ev);
	protected:
		bool OnMouseScroll(MouseScrollEvent& ev);
	};

}
