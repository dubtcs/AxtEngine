#include <pch.h>

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace axt {

	Camera::Camera(const glm::vec3& origin = glm::vec3{ 0.f,0.f,0.f }, const glm::vec3& direction = glm::vec3{ 1.f,0.f,0.f }) : position{ origin }, direction{ direction } {
		AXT_CORE_INFO("Bruh");
	}

	Camera::~Camera() {

	}

	// placeholder basic idea
	void Camera::Translate(const glm::vec3& translation) {
		position += translation;
	}

	void Camera::Rotate(const glm::vec3& rotation) {
		direction += rotation;
	}

	// Ortho
	OrthoCamera::OrthoCamera(const float& left, const float& right, const float& bottom, const float& top) : projection{ glm::ortho(left, right, bottom, top, -1.f, 1.f) }, view{ 1.f }, position{ 0.f } {
		viewProjection = projection * view;
	}

	OrthoCamera::~OrthoCamera() {

	}

	void OrthoCamera::SetPosition(const glm::vec3& newPosition) {
		position = newPosition;
		RemakeMatrices();
	}
	void OrthoCamera::SetRotation(const float& newrotation) {
		rotation = newrotation;
		RemakeMatrices();
	}
	void OrthoCamera::SetProjection(float l, float r, float b, float t) {
		projection = glm::ortho(l, r, b, t, -1.f, 1.f);
		viewProjection = projection * view;
	}

	const glm::vec3& OrthoCamera::GetPosition() const {
		return position;
	}

	const float& OrthoCamera::GetRotation() const {
		return rotation;
	}

	const glm::mat4& OrthoCamera::GetViewProjection() const {
		return viewProjection;
	}

	void OrthoCamera::RemakeMatrices() {
		glm::mat4 ones{ 1.f };
		glm::mat4 transformMatrix{ glm::translate(ones, position) * glm::rotate(ones, glm::radians(rotation), glm::vec3{0,0,1.f}) };
		view = glm::inverse(transformMatrix);
		viewProjection = projection * view;
	}

}