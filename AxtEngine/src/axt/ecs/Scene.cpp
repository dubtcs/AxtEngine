#include <pch.h>

#include "Scene.h"

namespace axt::ecs
{

	EntityID Scene::NewEntity()
	{
		return mIDManager.Create();
	}

	void Scene::DestroyEntity(EntityID& id)
	{
		mIDManager.Remove(id);
	}

}