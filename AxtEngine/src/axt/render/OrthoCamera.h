#pragma once

#include "axt/Core.h"

// temp
#include <glm/glm.hpp>

namespace axt {

	class AXT_API OrthoCamera {
	public:
		OrthoCamera(const float& left, const float& right, const float& bottom, const float& top);
		~OrthoCamera();
	public:
		void SetPosition(const glm::vec3& newPosition);
		void SetRotation(const float& newRotation);
		void SetProjection(float leftBound, float rightBound, float bottomBound, float topBound);
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
