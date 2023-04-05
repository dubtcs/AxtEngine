#include <pch.h>

#include "CameraControlSystem.h"

#include "Components.h"

#include "axt/core/Input.h"
#include "axt/core/Keycodes.h"

using namespace axt::ecs;

static constexpr float gMovementSpeed{ 1.f };
static constexpr float gZoomSpeed{ 0.1f };

static constexpr double gMaxZoom{ 25.f };
static constexpr double gMinZoom{ 0.25f };

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

		float lrbound{ camera.AspectRatio * camera.Zoom };
		camera.Projection = glm::ortho( -lrbound, lrbound, -camera.Zoom, camera.Zoom);
	}

	bool CameraControlSystem::OnEvent(Event& ev) {
		EventHandler handler{ ev };
		handler.Fire<MouseScrollEvent>(AXT_BIND_EVENT(CameraControlSystem::OnMouseScroll));

		return true;
	}

	bool CameraControlSystem::OnMouseScroll(MouseScrollEvent& ev) {
		SceneView<Camera> view{ mScene };
		for (EntityID id : view) {
			Camera& camera{ mScene->GetComponent<Camera>(id) };
			camera.Zoom = static_cast<float>(std::max(gMinZoom, std::min(gMaxZoom, camera.Zoom - (ev.GetY() * gZoomSpeed ))));
		}
		return false;
	}

	void CameraControlSystem::Resize(float x, float y) {
		float ratio{ x / y };
		// currently resizing every camera, might need a "resizable" flag
		SceneView<Camera> view{ mScene };
		for (EntityID id : view) {
			Camera& camera{ mScene->GetComponent<Camera>(id) };
			camera.AspectRatio = ratio;
		}
	}

}