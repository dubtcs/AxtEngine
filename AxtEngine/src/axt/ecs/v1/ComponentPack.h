#pragma once

#include "axt/Core.h"

#include "Types.h"

namespace axt::ecs
{

	using ComponentID = uint32_t;
	extern ComponentID sComponentTypeCount;

	template<typename T>
	ComponentID GetComponentTypeID()
	{
		// static variable will only be created once per unique typename T
		static ComponentID sTypeId{ sComponentTypeCount++ };
		return sTypeId;
	}

	// the void* will be wrapped in a template Get()
	class AXT_API ComponentPack
	{
	public:
		ComponentPack(size_t elementSize);
		~ComponentPack();
		void* Get(int32_t index);
	protected:
		char* mData;
		size_t mElementSize;
	};

}
