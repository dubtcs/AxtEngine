#pragma once

#include <axt/Core.h>

#include "System.h"
#include <necs/include.h>

namespace axt
{

	class AXT_API RenderSystem : protected System
	{
	public:
		bool OnUpdate(float dt);
		void SetActiveCamera(const necs::Entity& id);
		RenderSystem(Ref<necs::Scene>& scene);
	protected:
		necs::Entity mCamera;
	};

}
