#pragma once

#include <axt/Core.h>

#include "axt/ecs/nECS.h"
#include "System.h"

namespace axt
{

	class AXT_API RenderSystem
	{
	public:
		bool OnUpdate(float dt, const Ref<ecs::Scene>& scene);
		void SetActiveCamera(const ecs::EntityID& id);
		//RenderSystem(Ref<Scene>& scene);
	protected:
		ecs::EntityID mCameraID;
	};

}
