#include <pch.h>

#include "ComponentPack.h"

#include "Typedef.h"

#include <algorithm>

using DataIterator = std::vector<char>::iterator;

namespace axt::ecs
{

	ComponentPack::ComponentPack(size_t s) :
		mElementSize{ s }
	{
		mData = NewRef<std::vector<char>>();
		mIndexToEntity = NewRef<std::vector<EntityID>>();
		mEntityToIndex = NewRef<std::array<PackIndex, gMaxEntities>>();
		mEntityToIndex->fill(gMaxEntitiesOOB); // filled to invalidate every ID
	}

	void* ComponentPack::Get(const EntityID& id)
	{
		PackIndex index{ mEntityToIndex->at(id) };
		void* address{ &(mData->at(index * mElementSize)) };
		return address;
	}

	void* ComponentPack::Add(const EntityID& id)
	{
		mEntityToIndex->at(id) = mLength++;
		mIndexToEntity->push_back(id);
		mData->resize(mData->size() + mElementSize);
		return &(mData->at(mEntityToIndex->at(id) * mElementSize));
	}

	void ComponentPack::Remove(const EntityID& id)
	{
		if (mEntityToIndex->at(id) > gMaxEntities)
		{
			AXT_WARN("EntityID not used in pack");
			return;
		}

		PackIndex entityIndex{ mEntityToIndex->at(id) };
		EntityID lastEntity{ mIndexToEntity->back() };

		if ((mLength > 1) && (entityIndex < (mLength - 1)))
		{
			DataIterator rangeStart{ mData->begin() + (entityIndex * mElementSize) };
			DataIterator rangeEnd{ rangeStart + mElementSize };
			DataIterator replaceStart{ mData->end() - mElementSize };
			std::swap_ranges(rangeStart, rangeEnd, replaceStart);
			mEntityToIndex->at(lastEntity) = entityIndex;
			mIndexToEntity->at(entityIndex) = lastEntity;
		}

		mData->resize(mElementSize * --mLength);
		mEntityToIndex->at(id) = gMaxEntitiesOOB;
		mIndexToEntity->pop_back();
		return;

		/*
		KNOWN BUG:
		[fixed] Add 3 items, delete last 2, add 1, delete last, delete first - error
			also forgot to set mIndexToEntity to new EntityID lol
		*/
	}

}