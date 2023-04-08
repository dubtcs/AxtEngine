#include <pch.h>

#include "IDHandler.h"

#include <assert.h>

namespace necs
{

	static Entity GNewID{ nil + 1 };

	Entity IDHandler::Create()
	{
		if (mStack.empty())
		{
			if (GNewID >= GMaxEntitiesOOB)
				assert(false && "Too many entities in use");
			Entity id{ GNewID++ };
			mIdsUsed.push_back(id);
			return id;
		}
		else
		{
			Entity id{ mStack.top() };
			mStack.pop();
			mIdsUsed.push_back(id);
			return id;
		}
	}

	void IDHandler::Destroy(const Entity& id)
	{
		mStack.push(id);
		std::vector<Entity>::iterator index{ std::find(mIdsUsed.begin(), mIdsUsed.end(), id) };
		if (index != mIdsUsed.end())
		{
			*index = mIdsUsed.back();
			mIdsUsed.pop_back();
		}
	}

	IDHandler::IDHandler()
	{

	}

}
