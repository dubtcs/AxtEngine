#include <pch.h>

#include "GameWorld.h"

#include "components/all.h"

namespace axt
{

	using namespace necs;

	void GameWorld::CreateRootNode(const axt::UUID& worldRootID)
	{
		if (!mRootCreated)
		{
			mRootCreated = true;
			mWorldRoot = mScene->CreateEntity();
			GraphData& gdata{ mScene->Attach<GraphData>(mWorldRoot) };
			gdata.ID = worldRootID;
			mMap.emplace(worldRootID, mWorldRoot);
		}
		else
		{
			AXT_WARN("Root node already created for this world");
		}
	}

	GameWorld::GameWorld() :
		mScene{ NewRef<Scene>() }
	{
		axt::UUID rootID{ identify::GenerateUUID() };
		CreateRootNode(rootID);

		// Create a basic camera
		Entity camera{ CreateEntity() };
		mScene->Attach<Camera>(camera, (1920.f/1080.f));
		mScene->Attach<Transform>(camera);
		mScene->Attach<Description>(camera, { "StarterCamera" });
		mActiveCamera = camera;

		Entity cube{ CreateEntity() };
		Attach<Sprite>(cube, { { 0.1f, 0.7f, 0.7f, 1.f } });
		Attach<Transform>(cube);
		Attach<Description>(cube, { "StarterEntity" });
	}

	GameWorld::GameWorld(const axt::UUID& worldRootID) :
		mScene{ NewRef<Scene>() }
	{
		CreateRootNode(worldRootID);
	}

	Entity GameWorld::CreateEntity()
	{
		return CreateEntity(mWorldRoot);
	}

	Entity GameWorld::CreateEntity(const Entity& parent)
	{
		axt::UUID id{ identify::GenerateUUID() };
		return CreateEntityWithUUID(parent, id);
	}

	Entity GameWorld::CreateEntityWithUUID(const Entity& parent, const axt::UUID& id)
	{
		Entity e{ mScene->CreateEntity() };

		mMap.emplace(id, e);
		GraphData& pdata{ mScene->GetComponent<GraphData>(parent) };
		pdata.Children.push_back(id);

		mScene->Attach<GraphData>(e, GraphData{ pdata.ID, id });

		return e;
	}

	Entity GameWorld::LoadEntity(const axt::UUID& parent, const axt::UUID& id)
	{
		Entity e{ mScene->CreateEntity() };

		const Entity& pid{ GetEntityFromUUID(parent) };

		GraphData& pdata{ mScene->GetComponent<GraphData>(pid) };
		pdata.Children.push_back(id);

		mScene->Attach<GraphData>(e, GraphData{ parent, id });

		mMap.emplace(id, e);

		return e;
	}

	void GameWorld::DestroyEntity(const Entity& e)
	{
		if (mActiveCamera == e)
		{
			mActiveCamera = nil;
		}

		GraphData& gdata{ mScene->GetComponent<GraphData>(e) };
		for (axt::UUID& id : gdata.Children)
		{
			DestroyEntity(GetEntityFromUUID(id));
		}
		mMap.erase(gdata.ID);

		Entity pid{ GetEntityFromUUID(gdata.Parent) };
		GraphData& parent{ mScene->GetComponent<GraphData>(pid) };
		
		std::vector<axt::UUID>::iterator position{ std::find(parent.Children.begin(), parent.Children.end(), gdata.ID) };
		if (position != parent.Children.end())
		{
			parent.Children.erase(position);
		}
		
		mScene->DestroyEntity(e);
	}

	// Used for editor deletions where the iterator is known before the call
	/*void GameWorld::DestroyEntity(const Entity& e, const std::vector<Entity>::iterator& position)
	{

	}*/

}
