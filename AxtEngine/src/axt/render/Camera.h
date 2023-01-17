#pragma once

#include "axt/Core.h"

// temp
#include <glm/glm.hpp>

namespace axt {

	class AXT_API Camera {
	public:
		Camera(const glm::vec3& origin, const glm::vec3& direction);
		~Camera();
	public:
		void Translate(const glm::vec3& translation);
		void Rotate(const glm::vec3& rotation);
	protected:
		glm::vec3 position;
		glm::vec3 direction;
	};

	class AXT_API OrthoCamera {
	public:
		OrthoCamera(const float& left, const float& right, const float& bottom, const float& top);
		~OrthoCamera();
	public:
		void SetPosition(const glm::vec3& newPosition);
		void SetRotation(const float& newRotation);
		const glm::vec3& GetPosition() const;
		const float& GetRotation() const;
		const glm::mat4& GetViewProjection() const;
	protected:
		void RemakeMatrices();
	protected:
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 viewProjection;
		glm::vec3 position;
		float rotation{ 0.f };
	};

}
