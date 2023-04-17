#pragma once

#include <string>

struct Description
{
	std::string Name;
	Description(const std::string& name) : Name{ name } {}
	Description() = default;
};
