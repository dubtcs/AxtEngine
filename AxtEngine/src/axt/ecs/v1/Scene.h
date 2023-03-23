#pragma once

#include "axt/Core.h"

#include "IDManager.h"
#include "ComponentPack.h"

namespace axt::ecs
{

	class AXT_API Scene
	{
	public:
		EntityID NewEntity();
		void DestroyEntity(EntityID& id);
	public:
		template<typename T>
		Ref<T> Attach(const EntityID& id)
		{
			ComponentID cId{ GetComponentTypeID<T>() };
			if (cId >= mPacks.size())
			{
				mPacks.push_back(ComponentPack{ sizeof(T) });
			}
			mEntityInfo[id].Sig.set(cId);
			return NewRef<T>(mPacks[cId].Get(id));
		}
		template<typename T>
		void Detach(EntityID& id)
		{
			mEntityInfo[id].Sig.reset(GetComponentTypeID<T>());
		}
		template<typename T>
		Ref<T> GetComponentOfEntity(EntityID& id)
		{
			Ref<T> rComponent{ nullptr };
			ComponentID comId{ GetComponentTypeID<T>() };
			if (mEntityInfo[id].Sig.test(comId))
			{
				rComponent = NewRef<T>(mPacks[comId].Get(id));
			}
			return rComponent;
		}
	protected:
		struct EntityInfo
		{
			EntityID ID;
			Signature Sig;
		};
	protected:
		IDManager IDHandler;
		std::vector<EntityInfo> mEntityInfo;
		std::vector<ComponentPack> mPacks;
	};

}
