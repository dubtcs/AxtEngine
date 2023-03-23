#include <pch.h>

#include "EntityManager.h"

namespace axt::ecs
{


	EntityManager::EntityManager()
	{
		mEntityIDs.emplace(0);
	}

	EntityID EntityManager::Create()
	{
		EntityID id{ mEntityIDs.top() };
		mEntityIDs.pop();
		mEntityIDs.emplace(id + 1);
		mEntityAmount++;
		return id;
	}

	void EntityManager::Destroy(EntityID& e)
	{
		mEntityIDs.emplace(e);
		mSignatures[e].reset();
		mEntityAmount--;
		return;
	}

	Signature EntityManager::GetSig(EntityID& e)
	{
		Signature sig{ mSignatures[e] };
		return sig;
	}

	void EntityManager::SetSig(EntityID& e, Signature s)
	{
		mSignatures[e] = s;
		return;
	}

}
