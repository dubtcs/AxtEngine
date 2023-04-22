#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale{ 1.f };

	glm::mat4 ToMatrix()
	{
		glm::mat4 ones{ 1.f };
		return glm::translate(ones, Position) * glm::toMat4(glm::quat{ Rotation }) * glm::scale(ones, Scale);
	}
};
