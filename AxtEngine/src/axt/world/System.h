#pragma once

#include <axt/Core.h>
#include <axt/ecs/nECS.h>

#include "World.h"

namespace axt {
	class AXT_API System {
	public:
		System(Ref<World>& world) : mWorld{ world } {}
	protected:
		Ref<World> mWorld;
	};
}
