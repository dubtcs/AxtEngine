#pragma once

#include <necs/include.h>

#include "../UUID.h"

struct Heirarchy
{
	axt::UUID ID;
	necs::Entity Parent;
	std::vector<necs::Entity> Children;
	Heirarchy(necs::Entity parent) : Parent{ parent } {}
	Heirarchy() = default;
};
