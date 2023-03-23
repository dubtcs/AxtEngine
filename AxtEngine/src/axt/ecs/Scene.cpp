#include <pch.h>

#include "Scene.h"

namespace axt::ecs
{

	Scene::Scene()
	{
		mPacks = NewRef<std::vector<ComponentPack>>();
		mEntityInfo = NewRef<std::array<EntityInfo, gMaxEntities>>();
	}

	EntityID Scene::CreateEntity()
	{
		return mIDManager.Create();
	}

	void Scene::DestroyEntity(const EntityID& id)
	{
		// remove the data from each pack it belongs to
		// reset the mask
		for (ComponentPack& pack : *mPacks)
		{
			pack.Remove(id);
		}
		mEntityInfo->at(id).Mask.reset();
		mIDManager.Remove(id);
	}

}