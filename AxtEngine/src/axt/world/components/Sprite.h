#pragma once

#include <glm/glm.hpp>

struct Sprite
{
	glm::vec4 Color{ 0.5f, 0.5f, 0.5f, 1.f };
	glm::vec2 Size{ 1.f };
	Sprite(const glm::vec4& color, const glm::vec2& size = { 1.f,1.f }) : Color{ color }, Size{ size } {}
	Sprite() = default;
};
