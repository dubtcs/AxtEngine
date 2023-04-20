#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera
{
	float Zoom{ 1.f };
	float AspectRatio;
	bool Perspective{ false };
	glm::mat4 Projection;
	Camera() = default;
	Camera(float aspect_ratio) : Zoom{ 1.f }, AspectRatio { aspect_ratio }, Projection{ glm::ortho(-AspectRatio * Zoom, AspectRatio * Zoom, -Zoom, Zoom, -1.f, 1.f) } {}
};
