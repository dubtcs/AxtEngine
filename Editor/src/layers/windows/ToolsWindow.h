#pragma once

#include <axt/Core.h>

#include <axt/world/GameWorld.h>

namespace axt
{

	class AXT_API ToolsWindow
	{
	public:
		ToolsWindow() = default;
		void OnImGuiRender(Ref<GameWorld>& world);
	};

}
