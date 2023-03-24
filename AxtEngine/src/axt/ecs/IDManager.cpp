#include <pch.h>

#include "IDManager.h"

namespace axt::ecs
{

	IDManager::IDManager() :
		mIdsUsed{ 0 }
	{
		for (EntityID i{ gMaxEntities }; i > 0; i--)
			mStack.push(i - 1);
	}

	EntityID IDManager::Create()
	{
		EntityID r{ mStack.top() };
		mStack.pop();
		mIdsUsed++;
		return r;
	}

	void IDManager::Remove(const EntityID& id)
	{
		mStack.push(id);
		mIdsUsed--;
	}

}