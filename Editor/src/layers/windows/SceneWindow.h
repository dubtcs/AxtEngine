#pragma once

#include <axt/Core.h>

#include "axt/world/GameWorld.h"

#include <necs/include.h>

namespace axt
{

	class SceneOverviewWindow
	{
	public:
		SceneOverviewWindow() = default;
		necs::Entity OnImGuiRender(Ref<GameWorld>& world);
	protected:
		void DrawEntityTree(Ref<GameWorld>& world, const necs::Entity& entity);
	protected:
		necs::Entity mEntity{ necs::nil };
	};

}
