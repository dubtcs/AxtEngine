#pragma once

#include "axt/Core.h"

#include <glm/glm.hpp>

#include <bitset>
#include <queue>
#include <array>

namespace axt::ecs
{
	inline constexpr uint16_t gMaxEntities{ 5 }; // 100
	inline constexpr uint16_t gMaxComponents{ 32 }; // 16

	using Signature = std::bitset< gMaxComponents >;
	using EntityID = uint64_t;
	//using ComponentType = uint16_t;
}
