#pragma once

#include <axt/Core.h>
#include "Types.h"

#include <stack>

namespace axt::ecs
{

	// using a stack to keep the EntityID the same as it's index in the component arrays
	class AXT_API EntityManager
	{
	public:
		EntityManager();
		void Destroy(EntityID& e);
		void SetSig(EntityID& e, Signature s);
		EntityID Create();
		Signature GetSig(EntityID& e);
	protected:
		std::array<Signature, gMaxEntities> mSignatures;
		std::stack<EntityID> mEntityIDs;
		uint32_t mEntityAmount{ 0 };
	};

}
