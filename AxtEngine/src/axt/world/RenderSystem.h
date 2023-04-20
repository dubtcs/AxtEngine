#pragma once

#include <axt/Core.h>

#include "GameWorld.h"

#include <necs/include.h>

namespace axt
{

	namespace RenderSystem
	{
		bool OnUpdate(float dt, Ref<GameWorld> world);
	}

	/*class AXT_API RenderSystem
	{
	public:
		bool OnUpdate(float dt, Ref<GameWorld> world);
	};*/

	/*class AXT_API RenderSystem : protected System
	{
	public:
		bool OnUpdate(float dt);
		void SetActiveCamera(const necs::Entity& id);
		RenderSystem(Ref<necs::Scene>& scene);
	protected:
		necs::Entity mCamera;
	};*/

}
