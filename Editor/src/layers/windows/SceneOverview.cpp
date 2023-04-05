#include <pch.h>

#include "SceneOverview.h"

#include <axt/world/Components.h>

#include <imgui.h>

namespace axt
{

	using namespace ecs;

	SceneOverviewPanel::SceneOverviewPanel(const Ref<Scene>& scene) :
		mScene{ scene }
	{
		// bruh;
	}

	void SceneOverviewPanel::SetScene(const Ref<Scene>& scene)
	{
		mScene = scene;
	}

	void SceneOverviewPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Overview");

		SceneView<Description> view{ mScene };
		for (EntityID id : view)
		{
			Description& desc{ mScene->GetComponent<Description>(id) };
			ImGuiTreeNodeFlags flags{ ImGuiTreeNodeFlags_OpenOnArrow | ((mSelectedEntity == id) ? ImGuiTreeNodeFlags_Selected : 0) };
			bool open{ ImGui::TreeNodeEx((void*)id, flags, desc.Name.c_str()) };
			if (ImGui::IsItemClicked())
			{
				mSelectedEntity = id;
			}
			if (open)
			{
				if (mScene->HasComponent<Description>(id))
				{
					ImGui::Text("Foing");
				}
				ImGui::TreePop();
			}
		}

		ImGui::End();
	}

}
