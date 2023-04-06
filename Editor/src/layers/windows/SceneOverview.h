#pragma once

#include <axt/Core.h>
#include <axt/ecs/nECS.h>

#include <axt/world/World.h>

namespace axt
{

	class WorldOverviewPanel
	{
	public:
		WorldOverviewPanel() = default;
		WorldOverviewPanel(const Ref<World>& world);
		void SetWorld(const Ref<World>& world);
		ecs::EntityID OnImGuiRender();
	protected:
		void DrawChildTree(const ecs::EntityID& id);
		ecs::EntityID AddItem();
		ecs::EntityID AddItem(const ecs::EntityID& parent);
	protected:
		ecs::EntityID mSelectedEntity{ ecs::gMaxEntitiesOOB };
		Ref<World> mWorld;
	};

}
