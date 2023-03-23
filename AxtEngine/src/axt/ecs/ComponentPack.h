#pragma once

#include <axt/Core.h>
#include "Typedef.h"

namespace axt::ecs
{

	using PackIndex = int32_t;

	class AXT_API ComponentPack
	{
	public:
		ComponentPack() = default;
		ComponentPack(size_t elementSize) : mElementSize{ elementSize } { AXT_INFO("ComponentPack Created with ElementSize: {0}", elementSize); };
		PackIndex Add();
		void* Get(PackIndex& index);
		void Remove(PackIndex& index);

		// TEMP FOR DEBUGGING
		size_t GetSize() { return mData.size(); }
		template<typename T>
		T FetchAs(PackIndex& index)
		{
			return *(static_cast<T*>(Get(index)));
		}
	protected:
		//std::array<PackIndex, gMaxEntities> mEntityToIndex;

		// TODO: Make an array/map to associate the EntityID to the data index

		std::vector<char> mData;
		size_t mElementSize;
		uint32_t mLength{ 0 };
	};

}
