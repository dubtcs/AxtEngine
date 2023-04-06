#pragma once

#include <axt/Core.h>

#include "axt/ecs/nECS.h"
#include "System.h"

namespace axt
{

	class AXT_API RenderSystem : protected System
	{
	public:
		bool OnUpdate(float dt);
		void SetActiveCamera(const ecs::EntityID& id);
		RenderSystem(Ref<World>& world);
	protected:
		ecs::EntityID mCameraID;
	};

}
