#pragma once

#include <axt/Core.h>

#include <necs/include.h>

namespace axt
{

	class SceneOverviewWindow
	{
	public:
		SceneOverviewWindow() = default;
		SceneOverviewWindow(const Ref<necs::Scene>& scene);
		void SetScene(const Ref<necs::Scene>& scene);
		necs::Entity OnImGuiRender(const necs::Entity& world_root);
	protected:
		void DrawEntityTree(const necs::Entity& id);
		necs::Entity AddItem();
		necs::Entity AddItem(const necs::Entity& id);
	protected:
		Ref<necs::Scene> mScene;
		necs::Entity mEntity;
	};

}
