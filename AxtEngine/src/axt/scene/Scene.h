#pragma once

#include <axt/Core.h>

#include <entt/entt.hpp>

namespace axt {

	class AXT_API Scene {
	public:
		Scene() {};
		~Scene() {};
	protected:
		entt::registry mRegistry;
	};

}
