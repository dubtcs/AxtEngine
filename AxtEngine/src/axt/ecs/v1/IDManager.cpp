#include <pch.h>

#include "IDManager.h"

namespace axt::ecs
{

	IDManager::IDManager()
	{
		for (EntityID e{ gMaxEntities }; e > 0; e--)
			mIdStack.push(e - 1);
	}

	EntityID IDManager::New()
	{
		EntityID id{ mIdStack.top() };
		mIdStack.pop();
		mIdsUsed++;
		return id;
	}

	void IDManager::Remove(EntityID& id)
	{
		mIdStack.push(id);
		mIdsUsed--;
	}

}