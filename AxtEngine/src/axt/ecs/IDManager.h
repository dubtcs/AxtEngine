#pragma once

#include <axt/Core.h>

#include "Typedef.h"

#include <stack>

namespace axt::ecs
{

	class AXT_API IDManager
	{
	public:
		EntityID Create();
		void Remove(const EntityID& id);
		IDManager();
	protected:
		std::stack<EntityID> mStack;
		uint32_t mIdsUsed;
	};

}
