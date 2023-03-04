#include <pch.h>

#include "CameraController.h"

#include "axt/core/Input.h"
#include "axt/core/Keycodes.h"

namespace axt {

	OrthoCameraController::OrthoCameraController(float aspectRatio, bool rotation) : myAspectRatio{ aspectRatio }, myRotationEnabled{ rotation }, myCamera{ -myAspectRatio * myZoomLevel, myAspectRatio * myZoomLevel, -myZoomLevel, myZoomLevel } {
		// the foing
	}

	void OrthoCameraController::OnUpdate(float dt) {
		AXT_PROFILE_FUNCTION();

		// Translation
		if (input::IsKeyPressed(AXT_KEY_D)) {
			myCameraPosition.x += myCameraMovementSpeed * dt * myZoomLevel;
		} else if (input::IsKeyPressed(AXT_KEY_A)) {
			myCameraPosition.x -= myCameraMovementSpeed * dt * myZoomLevel;
		}
		if (input::IsKeyPressed(AXT_KEY_W)) {
			myCameraPosition.y += myCameraMovementSpeed * dt * myZoomLevel;
		} else if (input::IsKeyPressed(AXT_KEY_S)) {
			myCameraPosition.y -= myCameraMovementSpeed * dt * myZoomLevel;
		}

		// Rotation
		if (myRotationEnabled) {
			if (input::IsKeyPressed(AXT_KEY_Q)) {
				myCameraRotation += myCameraRotationSpeed * dt;
			}
			else if (input::IsKeyPressed(AXT_KEY_E)) {
				myCameraRotation -= myCameraRotationSpeed * dt;
			}
			myCamera.SetRotation(myCameraRotation);
		}
		
		myCamera.SetPosition(myCameraPosition);
	}

	void OrthoCameraController::OnEvent(Event& event) {
		AXT_PROFILE_FUNCTION();

		EventHandler handler{ event };
		handler.Fire<WindowResizeEvent>(AXT_BIND_EVENT(OrthoCameraController::OnWindowResize));
		handler.Fire<MouseScrollEvent>(AXT_BIND_EVENT(OrthoCameraController::OnMouseScroll));
	}

	const OrthoCamera& OrthoCameraController::GetCamera() const {
		return myCamera;
	}

	void OrthoCameraController::SetRotationEnabled(bool toggle) {
		myRotationEnabled = toggle;
	}

	void OrthoCameraController::Resize(float x, float y) {
		myAspectRatio = (x) / (y);
		myCamera.SetProjection(-myAspectRatio * myZoomLevel, myAspectRatio * myZoomLevel, -myZoomLevel, myZoomLevel);
	}

	// protected
	bool OrthoCameraController::OnWindowResize(WindowResizeEvent& ev) {
		AXT_PROFILE_FUNCTION();

		//Resize(static_cast<float>(ev.GetWidth()), static_cast<float>(ev.GetHeight()));
		return false;
	}

	bool OrthoCameraController::OnMouseScroll(MouseScrollEvent& event) {
		AXT_PROFILE_FUNCTION();

		myZoomLevel = (float)std::max(0.25, std::min(25.0, myZoomLevel - (event.GetY() * (double)myZoomSpeed)));
		myCamera.SetProjection(-myAspectRatio * myZoomLevel, myAspectRatio * myZoomLevel, -myZoomLevel, myZoomLevel);
		return false;
	}

}
