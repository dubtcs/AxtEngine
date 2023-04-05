#pragma once

#include <axt/Core.h>
#include <axt/ecs/nECS.h>

namespace axt
{

	class SceneOverviewPanel
	{
	public:
		SceneOverviewPanel() = default;
		SceneOverviewPanel(const Ref<ecs::Scene>& scene);
		void SetScene(const Ref<ecs::Scene>& scene);
		void OnImGuiRender();
	protected:
		ecs::EntityID mSelectedEntity;
		Ref<ecs::Scene> mScene;
	};

}
