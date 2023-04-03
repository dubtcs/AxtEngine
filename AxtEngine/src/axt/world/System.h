#pragma once

#include <axt/Core.h>
#include <axt/ecs/nECS.h>

namespace axt {
	class AXT_API System {
	public:
		System(Ref<ecs::Scene>& scene) : mScene{ scene } {}
	protected:
		Ref<ecs::Scene> mScene;
	};
}
