#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <axt/ecs/nECS.h>

namespace axt {

	struct Description
	{
		char Name[256];
		Description(const std::string& name) {
			if (name.size() < 256)
			{
				strncpy_s(Name, name.c_str(), name.size());
			}
			else
			{
				AXT_WARN("{0} | Name size overflow", __FUNCTION__);
			}
		}
		bool ChangeName(const std::string& name) {
			if (name.size() < 256)
			{
				strncpy_s(Name, name.c_str(), name.size());
				return true;
			}
			AXT_WARN("{0} | Name size overflow", __FUNCTION__);
			return false;
		}
	};

	// 3D space coordinates
	struct Position
	{
		glm::vec3 Value;
		float Static{ false }; // toggle gravity on/off
		Position() = default;
		Position(float value) : Value{ value } {}
		Position(float x, float y, float z) : Value{ x,y,z } {};
	};

	struct Color
	{
		glm::vec4 Value;
		Color(float c) : Value{ c, c, c, 1.f } {};
		Color(float r, float g, float b) : Value{ r, g, b, 1.f } {}
		Color(float r, float g, float b, float a) : Value{ r,g,b,a } {}
	};

	struct Rotation
	{
		glm::vec3 Value; // change to a quaternion to fix gimbal lock
		Rotation() = default;
		Rotation(float value) : Value{ value } {}
		Rotation(float x, float y, float z) : Value{ x,y,z } {};
	};

	struct Renderable
	{
		bool Value{ true };
	};

	struct Mesh
	{

	};

	struct Sprite
	{

	};

	struct Camera
	{
		float Zoom{ 1.f };
		float AspectRatio;
		glm::mat4 Projection;
		Camera(float aspectRatio) : Zoom{ 1.f }, AspectRatio { aspectRatio }, Projection{ glm::ortho(-AspectRatio * Zoom, AspectRatio * Zoom, -Zoom, Zoom, -1.f, 1.f) } {}
	};
}