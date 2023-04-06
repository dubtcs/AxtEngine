#include <pch.h>

#include "World.h"

#include "Components.h"

namespace axt
{

	using namespace ecs;

	World::World() :
		mScene{ NewRef<Scene>() }
	{
		mWorldRoot = mScene->CreateEntity();
		mActiveCamera = mScene->CreateEntity();

		mScene->Attach<Camera>(mActiveCamera, (1920.f / 1080.f));
		mScene->Attach<Position>(mActiveCamera, { 0.f, 0.f, 0.f });
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
		return CreateEntity(mWorldRoot);
	}

	EntityID World::CreateEntity(const EntityID& parent)
	{
		EntityID id{ mScene->CreateEntity() };
		CleanID(id);
		mHeirarchy[parent].Children.push_back(id);
		mHeirarchy[id].Parent = parent;
		return id;
	}

	void World::DestroyEntity(const EntityID& id)
	{
		CleanID(id);
		return mScene->DestroyEntity(id);
	}

	void World::CleanID(const EntityID& id)
	{
		if (!mHeirarchy[id].Children.empty())
		{
			for (EntityID& i : mHeirarchy[id].Children)
			{
				DestroyEntity(i);
			}
			mHeirarchy[id].Children.clear();
		}
		World::ChildGroup& c{ mHeirarchy[mHeirarchy[id].Parent].Children };
		World::ChildGroup::iterator position{ std::find(c.begin(), c.end(), id) };
		if (position != c.end())
		{
			c.erase(position);
		}
	}

}