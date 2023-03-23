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
