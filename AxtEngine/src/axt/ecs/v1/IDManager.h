#pragma once

#include "axt/Core.h"

#include "Types.h"

#include <stack>

namespace axt::ecs
{

	class AXT_API IDManager
	{
	public:
		EntityID New();
		void Remove(EntityID& id);
	public:
		IDManager();
	protected:
		std::stack<EntityID> mIdStack;
		uint64_t mIdsUsed{ 0 };
	};

}
