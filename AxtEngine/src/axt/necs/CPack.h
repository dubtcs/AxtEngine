#pragma once

#include "etypes.h"
#include <vector>
#include <array>
#include <unordered_map>

#include <assert.h>

namespace necs
{

	extern ComponentTypeID GComponentTypesRegistered;

	template<typename T>
	ComponentTypeID GetComponentTypeID()
	{
		static ComponentTypeID sTypeID{ GComponentTypesRegistered++ };
		return sTypeID;
	}

	class AComponentPack
	{
	public:
		AComponentPack() {};
		virtual ~AComponentPack() {};
		virtual void Destroy(const Entity& id) = 0;
	};

	template<typename T>
	class ComponentPack : public AComponentPack
	{
	public:
		ComponentPack() :
			mData{ MakeShared<std::vector<T>>() },
			mIndexToEntity{ MakeShared<std::vector<Entity>>() },
			mEntityToIndex{ MakeShared<std::unordered_map<Entity, PackIndex>>() }
			//mEntityToIndex{ MakeShared<std::array<PackIndex, GMaxEntities>>() },
		{
			//mEntityToIndex->fill(GMaxEntitiesOOB);
		}
		virtual ~ComponentPack() {}
	public:
		T& Get(const Entity& id)
		{

			if (mEntityToIndex->contains(id))
			{
				PackIndex index{ mEntityToIndex->at(id) };
				if (index < GMaxEntities)
				{
					return mData->at(index);
				}
			}
			assert(false && "entity does not contain component");
			return mScratchData;

			/*if (mEntityToIndex->at(id) < GMaxEntitiesOOB)
				return mData->at(mEntityToIndex->at(id));
			else
				return mScratchData;*/
		}

		template<typename... Ar>
		T& Add(const Entity& id, Ar&&... params)
		{
			mData->push_back({ params... });
			//mEntityToIndex->at(id) = mData->size() - 1;
			if (mEntityToIndex->contains(id))
			{
				mEntityToIndex->at(id) = mData->size() - 1;
			}
			else
			{
				mEntityToIndex->emplace(id, mData->size() - 1);
			}
			mIndexToEntity->push_back(id);
			return mData->at(mData->size() - 1);
		}
		
		virtual void Destroy(const Entity& id) override
		{
			if (id == nil)
				return;

			if (mEntityToIndex->contains(id))
			{
				PackIndex at{ mEntityToIndex->at(id) };
				if (at < GMaxEntitiesOOB)
				{
					Entity last{ mIndexToEntity->back() };

					if (mData->size() > 1 && (at < (mData->size() - 1)))
					{
						mData->at(at) = mData->back();
						mEntityToIndex->at(last) = at;
						mIndexToEntity->at(at) = last;
					}

					mData->pop_back();
					mIndexToEntity->pop_back();
					mEntityToIndex->at(id) = GMaxEntitiesOOB;
				}
			}
			return;

			/*PackIndex at{ mEntityToIndex->at(id) };

			if (at > GMaxEntities)
				return;

			Entity last{ mIndexToEntity->back() };

			if (mData->size() > 1 && (at < (mData->size() - 1)))
			{
				mData->at(at) = mData->back();
				mEntityToIndex->at(last) = at;
				mIndexToEntity->at(at) = last;
			}

			mData->pop_back();
			mIndexToEntity->pop_back();
			mEntityToIndex->at(id) = GMaxEntitiesOOB;*/

			//return;
		}

	protected:
		T mScratchData;
		Shared<std::vector<T>> mData;
		Shared<std::vector<Entity>> mIndexToEntity;
		Shared<std::unordered_map<Entity, PackIndex>> mEntityToIndex;
		//Shared<std::array<PackIndex, GMaxEntities>> mEntityToIndex;
	};

}
