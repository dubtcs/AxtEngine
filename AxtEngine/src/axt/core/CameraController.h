#pragma once

#include "axt/Core.h"

#include "axt/render/OrthoCamera.h"
#include "axt/events/WindowEvent.h"
#include "axt/events/MouseEvent.h"

namespace axt {

	class AXT_API OrthoCameraController {
	public:
		OrthoCameraController(float aspectRatio, bool rotation = false);
	public:
		void OnUpdate(float dt);
		void OnEvent(Event& event);
		const OrthoCamera& GetCamera() const;
		void SetRotationEnabled(bool toggle);
		void Resize(float x, float y);
	protected:
		bool OnWindowResize(WindowResizeEvent& event);
		bool OnMouseScroll(MouseScrollEvent& event);
	private:
		float myAspectRatio;
		bool myRotationEnabled;

		float myZoomLevel{ 1.f };
		float myZoomSpeed{ 0.1f };
		float myCameraRotation{ 0.f };
		float myCameraRotationSpeed{ 0.1f };
		float myCameraMovementSpeed{ 1.f };
		glm::vec3 myCameraPosition{ 0.f };
		OrthoCamera myCamera;
	};

	// should probably move this into a "3D Camera" file with the actual camera
	class AXT_API CameraController {

	};

}
