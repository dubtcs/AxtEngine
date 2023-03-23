#include <pch.h>

#include "ComponentPack.h"

#include "Typedef.h"

#include <algorithm>

namespace axt::ecs
{

	void* ComponentPack::Get(PackIndex& index)
	{
		return &mData[index * mElementSize];
	}

	PackIndex ComponentPack::Add()
	{
		// expanding buffer size to fit another element
		mData.resize(mData.size() + mElementSize);
		return mLength++;
	}

	void ComponentPack::Remove(PackIndex& index)
	{
		//char end{ mData[(mLength - 1) * mElementSize] };
		std::vector<char>::iterator fStartAddress{ mData.begin() + (index * mElementSize) };
		std::swap_ranges(fStartAddress, fStartAddress + mElementSize, mData.end() - mElementSize);
		mData.resize(mData.size() - mElementSize);
		mLength--;
	}

}