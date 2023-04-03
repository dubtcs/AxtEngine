#include <pch.h>

#include "CameraControlSystem.h"

#include "Components.h"

#include "axt/core/Input.h"
#include "axt/core/Keycodes.h"

using namespace axt::ecs;

static constexpr float gMovementSpeed{ 1.f };

namespace axt {

	CameraControlSystem::CameraControlSystem(Ref<Scene>& s) : System{ s } {}

	void CameraControlSystem::OnUpdate(float dt, EntityID id) {
		Position& position{ mScene->GetComponent<Position>(id) };
		Camera& camera{ mScene->GetComponent<Camera>(id) };

		// Translation
		if (input::IsKeyPressed(AXT_KEY_D)) {
			position.Value.x += gMovementSpeed * dt * (camera.Zoom);
		}
		else if (input::IsKeyPressed(AXT_KEY_A)) {
			position.Value.x -= gMovementSpeed * dt * (camera.Zoom);
		}
		if (input::IsKeyPressed(AXT_KEY_W)) {
			position.Value.y += gMovementSpeed * dt * (camera.Zoom);
		}
		else if (input::IsKeyPressed(AXT_KEY_S)) {
			position.Value.y -= gMovementSpeed * dt * (camera.Zoom);
		}
		camera.Projection = glm::ortho(-1.78f, 1.78f, -1.f, 1.f);
	}

	bool CameraControlSystem::OnMouseScroll(MouseScrollEvent& ev) {
		//myZoomLevel = (float)std::max(0.25, std::min(25.0, myZoomLevel - (event.GetY() * (double)myZoomSpeed)));
		//myCamera.SetProjection(-myAspectRatio * myZoomLevel, myAspectRatio * myZoomLevel, -myZoomLevel, myZoomLevel);

		return false;
	}

}