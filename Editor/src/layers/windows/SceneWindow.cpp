#include <pch.h>

#include "SceneWindow.h"

#include <imgui.h>
#include <axt/world/components/all.h>

namespace axt
{

	using namespace necs;

	static void AddCubeEntity(const Ref<GameWorld>& world, const Entity& parent)
	{
		Entity newEntity{ world->CreateEntity(parent) };
		world->Attach<Description>(newEntity, { "Entity" });
		world->Attach<Transform>(newEntity);
		world->Attach<Sprite>(newEntity);
	}

	static void AddCubeEntity(const Ref<GameWorld>& world)
	{
		Entity newEntity{ world->CreateEntity() };
		world->Attach<Description>(newEntity, { "Entity" });
		world->Attach<Transform>(newEntity);
		world->Attach<Sprite>(newEntity);
	}

	Entity SceneOverviewWindow::OnImGuiRender(Ref<GameWorld>& world)
	{
		ImGui::Begin("Scene Overview");

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::Button("Add Entity"))
			{
				AddCubeEntity(world);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		DrawEntityTree(world, world->GetWorldRoot());

		ImGui::End();

		return mEntity;
	}

	void SceneOverviewWindow::DrawEntityTree(Ref<GameWorld>& world, const necs::Entity& entity)
	{
		if (world->HasComponent<GraphData>(entity))
		{
			GraphData& gdata{ world->GetComponent<GraphData>(entity) };
			for (axt::UUID& id : gdata.Children)
			{
				Entity i{ world->GetEntityFromUUID(id) };
				if (world->HasComponent<Description>(i))
				{
					ImGuiTreeNodeFlags flags{ ImGuiTreeNodeFlags_OpenOnArrow | ((mEntity == i) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen };

					Description& d{ world->GetComponent<Description>(i) };
					bool open{ ImGui::TreeNodeEx((void*)i, flags, d.Name.c_str()) };

					if (ImGui::IsItemClicked())
					{
						mEntity = i;
					}

					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::Button("Add Child"))
						{
							AddCubeEntity(world, i);
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::Button("Delete"))
						{
							world->DestroyEntity(i);
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}

					if (open)
					{
						DrawEntityTree(world, i);
						ImGui::TreePop();
					}

				}
			}
		}

	}

}