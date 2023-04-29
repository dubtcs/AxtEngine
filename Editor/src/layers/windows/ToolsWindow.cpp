
#include "ToolsWindow.h"

#include <imgui.h>

namespace axt
{

	void ToolsWindow::OnImGuiRender(Ref<GameWorld>& world)
	{
		ImGui::Begin("Tools");

		// buttons to add standard objects light a cube, light, etc...

		ImGui::End();
	}

}