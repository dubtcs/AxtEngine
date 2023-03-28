#include <pch.h>

#include "IDManager.h"

namespace axt::ecs
{

	IDManager::IDManager() :
		mCount{ 0 }
	{
		for (EntityID i{ gMaxEntities }; i > 0; i--)
			mStack.push(i - 1);
	}

	EntityID IDManager::Create()
	{
		EntityID r{ mStack.top() };
		mIdsUsed.push_back(r);
		mStack.pop();
		mCount++;
		return r;
	}

	void IDManager::Remove(const EntityID& id)
	{
		std::vector<EntityID>::iterator index{ std::find(mIdsUsed.begin(), mIdsUsed.end(), id) };
		if (index != mIdsUsed.end())
		{
			mStack.push(id);
			mIdsUsed.erase(index);
		}
		mCount--;
	}

	bool IDManager::IsUsed(const EntityID& id) const
	{ // linear time ouch, maybe use a map
		if (std::find(mIdsUsed.begin(), mIdsUsed.end(), id) != mIdsUsed.end())
		{
			return true;
		}
		return false;
	}

}