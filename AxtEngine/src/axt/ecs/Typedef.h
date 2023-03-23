#pragma once

#include <memory>
#include <bitset>

namespace axt::ecs
{

	inline constexpr uint32_t gMaxEntities{ 1'000'000 }; // 100
	inline constexpr uint16_t gMaxComponents{ 32 }; // 16

	using Signature = std::bitset< gMaxComponents >;
	using EntityID = uint64_t;

	// make sure gMaxComponents and ComponentTypeID share same bit size
	using ComponentTypeID = uint32_t;

}
