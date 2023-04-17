#pragma once

#include <necs/include.h>

struct Heirarchy
{
	necs::Entity Parent;
	std::vector<necs::Entity> Children;
	Heirarchy(necs::Entity parent) : Parent{ parent } {}
	Heirarchy() = default;
};
