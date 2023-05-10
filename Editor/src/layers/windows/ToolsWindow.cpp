
#include "ToolsWindow.h"

#include <axt/render/Texture.h>

#include <imgui.h>

namespace axt
{

	static constexpr float ratio{ 0.1f };

	//static Ref<Texture2D> gButtonTexture{ Texture2D::Create("") };

	void ToolsWindow::OnImGuiRender(Ref<GameWorld>& world)
	{
		ImGui::SetNextWindowSizeConstraints({ 25.f, 100.f }, { 1000.f, 1000.f });
		ImGui::Begin("Tools");

		float height{ ImGui::GetContentRegionAvail().y };
		ImVec2 buttonSize{ height, height };

		// buttons to add standard objects like a cube, light, etc...
		ImGui::Button("New Item", buttonSize);

		ImGui::End();
	}

}