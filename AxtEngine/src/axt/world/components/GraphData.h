#pragma once

#include "../UUID.h"

#include <vector>

struct GraphData
{
	axt::UUID ID;
	axt::UUID Parent;
	std::vector<axt::UUID> Children;
	GraphData(const axt::UUID& parent) : ID{ axt::identify::GenerateUUID() }, Parent{ parent } {}
	GraphData(const axt::UUID& parent, const axt::UUID& id) : ID{ id }, Parent{ parent } {}
	GraphData() = default;
};
