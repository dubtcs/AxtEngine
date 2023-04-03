#include <pch.h>

#include "World.h"

#include "Components.h"

namespace axt
{

	using namespace ecs;

	World::World() :
		mScene{ NewRef<Scene>() },
		mActiveCamera{ mScene->CreateEntity() }
	{
		mScene->Attach<Camera>(mActiveCamera);
		mScene->Attach<Position>(mActiveCamera, { 0.f });
	}

	World::~World()
	{

	}

	bool World::OnUpdate(float dt)
	{
		// render

		return true;
	}

	EntityID World::CreateEntity()
	{
		return mScene->CreateEntity();
	}

	void World::DestroyEntity(const EntityID& id)
	{
		return mScene->DestroyEntity(id);
	}

}