#pragma once

#include <axt/Core.h>

#include <necs/include.h>

namespace axt
{

	class AXT_API ToolsWindow
	{
	public:
		ToolsWindow(const Ref<necs::Scene>& scene);
	public:
		void OnImGuiRender(const necs::Entity& root);
	protected:
		Ref<necs::Scene> mScene;
	};

}
