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
		EntityID CreateEntity();
		void DestroyEntity(const EntityID& id);
		std::bitset<gMaxComponents>& GetBitset(EntityID& id)
		{
			return mEntityInfo->at(id).Mask;
		}
		template<typename T>
		void Attach(const EntityID& id)
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };

			// make a ComponentPack for this attachment if none exists
			if (mPacks->size() <= cid)
			{
				mPacks->resize(cid); // resize it to the ComponentID, that way we can use push_back to just poop one out at the end
				mPacks->push_back({ sizeof(T) });
			}

			mPacks->at(cid).Add(id);

			mEntityInfo->at(id).Mask.set(cid);
		}
		template<typename T>
		void Detach(const EntityID& id)
		{
			mEntityInfo->at(id).Mask.reset(GetComponentTypeID<T>());
		}
		Scene();
	public:
		struct EntityInfo
		{
			EntityID ID;
			Signature Mask;
			//bool IsValid{ true }; // switch this to false when the data pocket moves or entity is deleted
		};
	protected:
		IDManager mIDManager;
		Ref<std::vector<ComponentPack>> mPacks;
		Ref<std::array<EntityInfo, gMaxEntities>> mEntityInfo;
	};

}
