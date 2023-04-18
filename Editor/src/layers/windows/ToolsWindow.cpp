#include <pch.h>

#include "ToolsWindow.h"

#include <imgui.h>

namespace axt
{

	using namespace necs;

	ToolsWindow::ToolsWindow(const Ref<Scene>& scene) :
		mScene{ scene }
	{

	}

	void ToolsWindow::OnImGuiRender(const Entity& root)
	{
		ImGui::Begin("Tools");

		ImGui::Button("New Item");

		ImGui::End();
	}

}