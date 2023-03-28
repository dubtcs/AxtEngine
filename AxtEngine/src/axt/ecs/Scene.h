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
		struct EntityInfo
		{
			EntityID ID;
			Signature Mask;
		};
	public:
		EntityID CreateEntity();
		void DestroyEntity(const EntityID& id);
		Ref<std::array<EntityInfo, gMaxEntities>> GetEntityInfo();
		const IDManager& GetIDManager() const;

		template<typename T>
		T& Attach(const EntityID& id) // Initialize entry with blank data
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };

			// make a ComponentPack for this attachment if none exists
			if (mPacks->size() <= cid)
			{
				mPacks->resize(cid); // resize it to the ComponentID, that way we can use push_back to just poop one out at the end
				mPacks->push_back({ sizeof(T) });
			}


			mEntityInfo->at(id).Mask.set(cid);
			return *static_cast<T*>(mPacks->at(cid).Add(id));
		}
		template<typename T>
		T& Attach(const EntityID& id, const T& copycat) // Initialize entry with an existing object
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };

			// make a ComponentPack for this attachment if none exists
			if (mPacks->size() <= cid)
			{
				mPacks->resize(cid); // resize it to the ComponentID, that way we can use push_back to just poop one out at the end
				mPacks->push_back({ sizeof(T) });
			}

			mEntityInfo->at(id).Mask.set(cid);
			T& data{ *static_cast<T*>(mPacks->at(cid).Add(id)) };
			data = copycat;
			return data;
		}
		template<typename T, typename... Ar>
		T& Attach(const EntityID& id, Ar&&... params) // Initialize entry with parameter pack
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };

			// make a ComponentPack for this attachment if none exists
			if (mPacks->size() <= cid)
			{
				mPacks->resize(cid); // resize it to the ComponentID, that way we can use push_back to just poop one out at the end
				mPacks->push_back({ sizeof(T) });
			}

			mEntityInfo->at(id).Mask.set(cid);
			T& data{ *static_cast<T*>(mPacks->at(cid).Add(id)) };
			data = T{ params... };
			return data;
		}
		template<typename T>
		void Detach(const EntityID& id)
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };
			mEntityInfo->at(id).Mask.reset(cid);
			mPacks->at(cid).Remove(id);
		}

		// returns a pointer to the data block
		template<typename T>
		T& GetComponent(const EntityID& id)
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };

			if (mPacks->size() <= cid)
			{
				assert(false && "No component pack found");
				//std::quick_exit(EXIT_FAILURE);
			}

			return *(static_cast<T*>(mPacks->at(cid).Get(id)));
		}

		Scene();
	protected:
		IDManager mIDManager;
		Ref<std::vector<ComponentPack>> mPacks;
		Ref<std::array<EntityInfo, gMaxEntities>> mEntityInfo;
	};

	// Wrapper for scene functions
	class AXT_API Entity
	{
	public:
		Entity(Ref<Scene>& s);
		~Entity();
		void Destroy();
		template<typename T>
		T& Attach()
		{
			return mScene->Attach<T>(mID);
		}
		template<typename T>
		void Detach()
		{
			return mScene->Detach<T>(mID);
		}
		template<typename T>
		T& GetComponent()
		{
			return mScene->GetComponent<T>(mID);
		}
	protected:
		EntityID mID;
		Scene* mScene;
		//Signature mSignature;
	};

}
