#pragma once

#include <axt/Core.h>

#include <necs/include.h>

namespace axt
{

	class PropertiesWindow
	{
	public:
		PropertiesWindow() = default;
		PropertiesWindow(const Ref<necs::Scene>& scene);
		void SetScene(const Ref<necs::Scene>& scene);
		void OnImGuiRender(const necs::Entity& id);
		void DrawEntityInfo(const necs::Entity& id);
	protected:
		Ref<necs::Scene> mScene;
	};

}
