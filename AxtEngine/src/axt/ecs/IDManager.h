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
		bool IsUsed(const EntityID& id) const;
		IDManager();
	public:
		const std::vector<EntityID>::const_iterator begin() const { return mIdsUsed.begin(); }
		const std::vector<EntityID>::const_iterator end() const { return mIdsUsed.end(); }
	protected:
		std::stack<EntityID> mStack;
		std::vector<EntityID> mIdsUsed;
		uint32_t mCount;
	};

}
