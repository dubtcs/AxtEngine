#pragma once

#include <axt/Core.h>
#include "../assets/Model.h"
#include <glm/glm.hpp>

struct Mesh
{
	glm::vec4 Color{ 0.5f, 0.5f, 0.5f, 1.f };
	glm::vec3 Size{ 1.f };
	axt::Ref<axt::Model> Model;
	// std::string MeshPath;
	Mesh(const glm::vec4& color, const glm::vec3& size = { 1.f, 1.f, 1.f }) : Color{ color }, Size{ size } {}
	Mesh() = default;
};
