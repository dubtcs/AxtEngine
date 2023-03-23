#pragma once

#include <axt/Core.h>

namespace axt::ecs
{

	using PackIndex = int32_t;

	class AXT_API ComponentPack
	{
	public:
		PackIndex Add();
		void* Get(PackIndex& index);
		void Remove(PackIndex& index);
		ComponentPack(size_t elementSize) : mElementSize{ elementSize } {};

		// TEMP FOR DEBUGGING
		size_t GetSize() { return mData.size(); }
		template<typename T>
		T FetchAs(PackIndex& index)
		{
			return *(static_cast<T*>(Get(index)));
		}
	protected:
		std::vector<char> mData;
		size_t mElementSize;
		uint32_t mLength{ 0 };
	};

}
