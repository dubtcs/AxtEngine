#include <pch.h>

#include "SceneOverview.h"

#include <axt/world/Components.h>

#include <imgui.h>

namespace axt
{

	using namespace ecs;

	using HeirarchyIterator = std::vector<EntityID>::iterator;

	WorldOverviewPanel::WorldOverviewPanel(const Ref<World>& scene) :
		mWorld{ scene }
	{
		// bruh;
	}

	void WorldOverviewPanel::SetWorld(const Ref<World>& scene)
	{
		mWorld = scene;
	}

	EntityID WorldOverviewPanel::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("World Overview");

		if (ImGui::Button("Add Item"))
		{
			EntityID id{ AddItem() }; // this will add to the world root
		}

		ImGui::Separator();

		DrawChildTree(mWorld->GetRoot());

		ImGui::End();
		return mSelectedEntity;
	}

	EntityID WorldOverviewPanel::AddItem()
	{
		return AddItem(mWorld->GetRoot());
	}

	EntityID WorldOverviewPanel::AddItem(const EntityID& parent)
	{
		ecs::EntityID id{ mWorld->CreateEntity(parent) };
		mWorld->Attach<Position>(id, 0.25f, 0.25f, 0.5f);
		mWorld->Attach<Color>(id, 0.25f, 0.25f, 0.25f);
		mWorld->Attach<Renderable>(id);
		mWorld->Attach<Description>(id, { "Bruh" });
		return id;
	}

	void WorldOverviewPanel::DrawChildTree(const EntityID& parent)
	{
		for (EntityID id : mWorld->GetChildren(parent))
		{
			if (mWorld->HasComponent<Description>(id))
			{
				Description& d{ mWorld->GetComponent<Description>(id) };

				ImGuiTreeNodeFlags flags{ ImGuiTreeNodeFlags_OpenOnArrow | ((mSelectedEntity == id) ? ImGuiTreeNodeFlags_Selected : 0) };
				bool open{ ImGui::TreeNodeEx((void*)id, flags, d.Name) };

				if (ImGui::IsItemClicked())
				{
					mSelectedEntity = id;
				}

				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::Button("Add Child"))
					{
						EntityID newID{ AddItem(id) };
						ImGui::CloseCurrentPopup();
					}
					if (ImGui::Button("Delete"))
					{
						mWorld->DestroyEntity(id);
						mSelectedEntity = gMaxEntitiesOOB;
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				if (open)
				{
					DrawChildTree(id);
					ImGui::TreePop();
				}

			}
		}

	}

}

//for (EntityID id : *h.Children)
		//{
		//	//EntityID id{ *i };
		//	if (mWorld->HasComponent<Description>(id) && mWorld->HasComponent<Heirarchy>(id))
		//	{
		//		Description& desc{ mWorld->GetComponent<Description>(id) };
		//		//Heirarchy& h{ mWorld->GetComponent<Heirarchy>(id) };

		//		ImGuiTreeNodeFlags flags{ ImGuiTreeNodeFlags_OpenOnArrow | ((mSelectedEntity == id) ? ImGuiTreeNodeFlags_Selected : 0) };
		//		bool open{ ImGui::TreeNodeEx((void*)id, flags, desc.Name) };
		//		if (ImGui::IsItemClicked())
		//		{
		//			mSelectedEntity = id;
		//		}
		//		if (ImGui::BeginPopupContextItem())
		//		{
		//			if (ImGui::Button("Add Child"))
		//			{
		//				EntityID newID{ AddItem(id) };
		//				//h.Children->push_back(newID);
		//				ImGui::CloseCurrentPopup();
		//			}
		//			if (ImGui::Button("Delete"))
		//			{
		//				mWorld->DestroyEntity(id);
		//				mSelectedEntity = gMaxEntitiesOOB;
		//				ImGui::CloseCurrentPopup();
		//			}
		//			ImGui::EndPopup();
		//		}
		//		if (open)
		//		{
		//			/*for (EntityID id : *h.Children)
		//			{
		//				ImGui::Text("CHILD");
		//			}*/
		//			ImGui::TreePop();
		//		}
		//	}
		//}
