#pragma once

#include <axt/Core.h>
#include <axt/ecs/nECS.h>

#include <axt/world/World.h>

namespace axt
{

	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;
		PropertiesPanel(const Ref<World>& world);
		void SetWorld(const Ref<World>& world);
		void OnImGuiRender(const ecs::EntityID& id);
		void DrawEntityInfo(const ecs::EntityID& id);
	protected:
		Ref<World> mWorld;
	};

}
