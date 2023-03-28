#include <pch.h>

#include "ComponentPack.h"

#include "Typedef.h"

#include <algorithm>

inline constexpr size_t gMinVectorCapacity{ 25 };

namespace axt::ecs
{

	ComponentPack::ComponentPack(size_t s) :
		mElementSize{ s }
	{
		//mData = NewRef<std::vector<char>>(gMinVectorCapacity * s);
		//mIndexToEntity = NewRef<std::vector<size_t>>(gMinVectorCapacity);
		// try a hash map-esque way of packing data, reserve a minimum amount of data, say 20% max components, then expand if we need too

		mData = NewRef<std::vector<char>>();
		mIndexToEntity = NewRef<std::vector<size_t>>();
		mEntityToIndex = NewRef<std::array<size_t, gMaxEntities>>();
		mEntityToIndex->fill(gMaxEntitiesOOB); // filled to invalidate every ID
	}

	void* ComponentPack::Get(const EntityID& id)
	{
		size_t index{ mEntityToIndex->at(id) };
		void* address{ &(mData->at(index * mElementSize)) };
		return address;
	}

	void* ComponentPack::Add(const EntityID& id)
	{
		// expanding buffer size to fit another element
		mData->resize(mData->size() + mElementSize);

		mEntityToIndex->at(id) = mLength;
		mIndexToEntity->push_back(id);

		return &(mData->at(mLength++ * mElementSize));
	}

	void ComponentPack::Remove(const EntityID& id)
	{
		size_t entityIndex{ mEntityToIndex->at(id) };
		EntityID lastEntity{ mIndexToEntity->back() };

		if (entityIndex == gMaxEntitiesOOB)
		{
			AXT_WARN("OOB {0}", entityIndex);
			return;
		}

		if (lastEntity != id)
		{
			std::vector<char>::iterator startRange{ mData->begin() + (entityIndex * mElementSize) };
			std::vector<char>::iterator endRange{ startRange + mElementSize };
			std::vector<char>::iterator endStart{ mData->end() - mElementSize };
			std::swap_ranges(startRange, endRange, endStart);

			mEntityToIndex->at(lastEntity) = entityIndex;
		}

		std::vector<char>::iterator it{ std::find(mData->begin(), mData->end(), entityIndex) };
		if (it != mData->end())
		{
			mData->erase(it);
		}

		mEntityToIndex->at(id) = gMaxEntitiesOOB;
		mData->resize(mLength-- * mElementSize);

		/*
		size_t replacementIndex{ mEntityToIndex->at(id) };

		if (replacementIndex > gMaxEntities)
		{
			return;
		}

		EntityID lastEntity{ mIndexToEntity->at(mLength - 1) };

		if (lastEntity != id)
		{
			size_t offset{ mEntityToIndex->at(id) * mElementSize };
			std::vector<char>::iterator start{ mData->begin() + offset };
			std::swap_ranges(start, start + mElementSize, mData->end() - mElementSize);
			mEntityToIndex->at(lastEntity) = replacementIndex;
			mIndexToEntity->at(replacementIndex) = lastEntity;
		}

		// pushes the index past the max available to invalidate the ID
		mEntityToIndex->at(id) = gMaxEntitiesOOB;

		mLength--;
		mData->resize(mLength * mElementSize);
		*/
	}

}