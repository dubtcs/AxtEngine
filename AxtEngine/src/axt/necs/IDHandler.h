#pragma once

#include "etypes.h"

#include <vector>
#include <stack>
#include <unordered_set>

namespace necs
{

	class IDHandler
	{
	public:
		IDHandler();
		Entity Create();
		void Destroy(const Entity& id);
	public:
		const std::vector<Entity>::const_iterator begin() const { return mIdsUsed.begin(); }
		const std::vector<Entity>::const_iterator end() const { return mIdsUsed.end(); }
	protected:
		std::vector<Entity> mIdsUsed;
		//std::unordered_set<Entity> mIdsUsed;
		std::stack<Entity> mStack;
	};

}
