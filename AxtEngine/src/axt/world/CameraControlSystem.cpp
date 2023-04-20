#include <pch.h>

#include "CameraControlSystem.h"

#include "components/all.h"

#include <axt/core/Input.h>
#include "axt/core/Keycodes.h"

static constexpr float gMovementSpeed{ 1.f };
static constexpr float gZoomSpeed{ 0.1f };

static constexpr double gMaxZoom{ 25.f };
static constexpr double gMinZoom{ 0.25f };

namespace axt
{

	using namespace necs;

	static float gAspectRatio;

	void CameraControlSystem::OnUpdate(float dt, Ref<GameWorld> world)
	{
		Entity cameraID{ world->GetActiveCamera() };
		Camera& camera{ world->GetComponent<Camera>(cameraID) };
		Transform& transform{ world->GetComponent<Transform>(cameraID) };

		camera.AspectRatio = gAspectRatio;

		if (input::IsKeyPressed(AXT_KEY_D))
		{
			transform.Position.x += gMovementSpeed * dt * (camera.Zoom);
		}
		else if (input::IsKeyPressed(AXT_KEY_A)) 
		{
			transform.Position.x -= gMovementSpeed * dt * (camera.Zoom);
		}
		if (input::IsKeyPressed(AXT_KEY_W)) 
		{
			transform.Position.y += gMovementSpeed * dt * (camera.Zoom);
		}
		else if (input::IsKeyPressed(AXT_KEY_S)) 
		{
			transform.Position.y -= gMovementSpeed * dt * (camera.Zoom);
		}

		float lrbound{ camera.AspectRatio * camera.Zoom };
		camera.Projection = glm::ortho(-lrbound, lrbound, -camera.Zoom, camera.Zoom);
	}

	void CameraControlSystem::OnResize(float x, float y)
	{
		gAspectRatio = x / y;
	}

	//bool CameraControlSystem::OnEvent(Event& ev)
	//{
	//	EventHandler handler{ ev };
	//	handler.Fire<MouseScrollEvent>(AXT_BIND_EVENT(CameraControlSystem::OnMouseScroll));

	//	return false;
	//}

	//bool CameraControlSystem::OnMouseScroll(MouseScrollEvent& ev)
	//{
	//	SceneView<Camera> view{ mScene };
	//	for (Entity id : view) {
	//		Camera& camera{ mScene->GetComponent<Camera>(id) };
	//		camera.Zoom = static_cast<float>(std::max(gMinZoom, std::min(gMaxZoom, camera.Zoom - (ev.GetY() * gZoomSpeed))));
	//	}
	//	return false;
	//}

}