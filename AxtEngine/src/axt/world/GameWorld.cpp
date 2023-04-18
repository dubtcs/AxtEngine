#include <pch.h>

#include "GameWorld.h"

#include "components/all.h"

namespace axt
{

	using namespace necs;

	GameWorld::GameWorld() :
		mScene{ NewRef<Scene>() }
	{
		mWorldRoot = mScene->CreateEntity();
	}

	Entity GameWorld::CreateEntity(const Entity& parent)
	{
		Entity e{ mScene->CreateEntity() };
		Heirarchy& h{ mScene->Attach<Heirarchy>(e) };
		h.Parent = parent;
		return e;
	}

	void GameWorld::DestroyEntity(const Entity& e)
	{
		mScene->DestroyEntity(e);
	}

	/*bool GameWorld::ChangeParent(const Entity& e, const Entity& newParent)
	{
		Heirarchy& h1{ mScene->GetComponent<Heirarchy>(e) };
		Heirarchy& rh{ mScene->GetComponent<Heirarchy>(h1.Parent) };

		std::vector<Entity>::iterator index{ std::find(rh.Children.begin(), rh.Children.end(), e) };
		if (index != rh.Children.end())
		{
			rh.Children.erase(index);
		}

		h1.Parent = newParent;
		Heirarchy& h2{ mScene->GetComponent<Heirarchy>(newParent) };
		h2.Children.push_back(e);
		return true;
	}*/

}
