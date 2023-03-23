#include <pch.h>

#include "ComponentPack.h"

namespace axt::ecs
{
	ComponentID sComponentTypeCount{ 0 };

	ComponentPack::ComponentPack(size_t elementSize) : 
		mElementSize{ elementSize }
	{
		mData = new char[elementSize * gMaxEntities];
	}

	ComponentPack::~ComponentPack()
	{
		delete[] mData;
	}

	void* ComponentPack::Get(int32_t index)
	{
		return mData + (index * mElementSize);
	}

}
