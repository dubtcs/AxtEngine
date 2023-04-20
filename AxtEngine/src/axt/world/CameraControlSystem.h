#pragma once

#include <axt/Core.h>

#include "GameWorld.h"

#include "System.h"
#include "axt/events/Event.h"
#include "axt/events/WindowEvent.h"
#include "axt/events/MouseEvent.h"

#include <necs/include.h>

namespace axt
{

	namespace CameraControlSystem
	{

		void OnUpdate(float dt, Ref<GameWorld> world);
		void OnResize(float x, float y);

	}

	/*class AXT_API CameraControlSystem : protected System
	{
	public:
		CameraControlSystem(Ref<necs::Scene>& scene);
		void OnUpdate(float dt, const necs::Entity& cameraEntity);
		void OnResize(float x, float y);
		bool OnEvent(Event& ev);
	protected:
		bool OnMouseScroll(MouseScrollEvent& ev);
	};*/

}
