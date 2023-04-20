#pragma once

#include <axt/Core.h>

// This isn't a real UUID for now. Just a placeholder to get GameWorld working
// Real GUID/UUID is 128 bit

namespace axt
{

	using UUID = uint64_t;

	namespace identify
	{

		UUID GenerateUUID();

	}

}
