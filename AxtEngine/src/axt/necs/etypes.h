#pragma once

#include <memory>
#include <bitset>

namespace necs
{
	constexpr int64_t GMaxComponents{ 64 };
	constexpr int64_t GMaxEntities{ 1'000'000 };
	constexpr int64_t GMaxEntitiesOOB{ GMaxEntities + 1 };

	using Entity = uint32_t;
	using PackIndex = size_t;
	using Signature = std::bitset<GMaxComponents>;
	using ComponentTypeID = uint32_t;

	constexpr Entity nil = 0;

	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T, typename... A>
	constexpr Shared<T> MakeShared(A&&... a)
	{
		return std::make_shared<T>(std::forward<A>(a)...);
	}

}
