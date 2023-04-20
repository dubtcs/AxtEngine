#include <pch.h>

#include "UUID.h"

#include <random>

namespace axt::identify
{

	static std::random_device gRandomDevice;
	static std::mt19937_64 gMtEngine{ gRandomDevice() };
	static std::uniform_int_distribution<UUID> gDistribution;

	UUID GenerateUUID()
	{
		return gDistribution(gMtEngine);
	}

}