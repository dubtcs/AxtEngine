#pragma once

#include <axt/Core.h>
#include "Typedef.h"

#include <ostream>

namespace axt::ecs
{

	using PackIndex = int32_t;

	class AXT_API ComponentPack
	{
	public:
		ComponentPack() = default;
		ComponentPack(size_t elementSize);
		void* Get(const EntityID& id);
		void Add(const EntityID& id);
		void Remove(const EntityID& id);

		// TEMP FOR DEBUGGING
		size_t GetSize() { return mData->size(); }
		template<typename T>
		T FetchAs(PackIndex& index)
		{
			return *(static_cast<T*>(Get(index)));
		}
		template<typename T>
		T FetchAs(const EntityID& id)
		{
			return *(static_cast<T*>(Get(id)));
		}

	protected:
		Ref<std::vector<char>> mData;
		Ref<std::vector<size_t>> mIndexToEntity;
		Ref<std::array<size_t, gMaxEntities>> mEntityToIndex;
		size_t mElementSize;
		uint32_t mLength{ 0 };
	};

}
