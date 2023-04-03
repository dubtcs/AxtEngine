#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	glm::mat4 Projection;
	Camera(float left, float right, float bottom, float top) : Projection{ glm::ortho(left, right, bottom, top, -1.f, 1.f) } {};
};
