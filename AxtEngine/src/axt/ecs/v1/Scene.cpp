#include <pch.h>

#include "Scene.h"

namespace axt::ecs
{

	EntityID Scene::NewEntity()
	{
		EntityID id{ IDHandler.New() };
		mEntityInfo.push_back({ id, 0 });
		return id;
	}

	void Scene::DestroyEntity(EntityID& id)
	{
		IDHandler.Remove(id);
	}

}