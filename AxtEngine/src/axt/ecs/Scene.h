#pragma once

#include <axt/Core.h>

#include "IDManager.h"
#include "ComponentPack.h"
#include "Components.h"

#include <vector>

namespace axt::ecs
{

	class AXT_API Scene
	{
	public:
		EntityID NewEntity();
		void DestroyEntity(EntityID& id);
		std::bitset<gMaxComponents>& GetBitset(EntityID& id)
		{
			return mEntityInfo[id].Mask;
		}
		template<typename T>
		void Attach(EntityID& id)
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };

			// make a ComponentPack for this attachment if none exists
			if (mPacks.size() <= cid)
			{
				mPacks.resize(cid); // resize it to the ComponentID, that way we can use push_back to just poop one out at the end
				mPacks.push_back({ sizeof(T) });
			}

			PackIndex i{ mPacks[cid].Add() };

			mEntityInfo[id].Mask.set(cid);
		}
		template<typename T>
		void Detach(EntityID& id)
		{
			mEntityInfo[id].Mask.reset(GetComponentTypeID<T>());
		}
	public:
		struct EntityInfo
		{
			EntityID ID;
			Signature Mask;
			//bool IsValid{ true }; // switch this to false when the data pocket moves or entity is deleted
		};
	protected:
		IDManager mIDManager;
		std::vector<ComponentPack> mPacks;
		std::array<EntityInfo, gMaxEntities> mEntityInfo{};
	};

}
