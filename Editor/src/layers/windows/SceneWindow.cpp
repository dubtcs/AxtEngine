#include <pch.h>

#include "SceneWindow.h"

#include <imgui.h>
#include <axt/world/components/all.h>

namespace axt
{

	using namespace necs;

	SceneOverviewWindow::SceneOverviewWindow(const Ref<Scene>& scene) :
		mScene{ scene },
		mEntity{ nil }
	{

	}

	Entity SceneOverviewWindow::OnImGuiRender(const Entity& root)
	{
		ImGui::Begin("Scene Overview");

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::Button("Add Entity"))
			{
				AddItem(root);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		DrawEntityTree(root);

		ImGui::End();

		return mEntity;
	}

	void SceneOverviewWindow::SetScene(const Ref<Scene>& scene)
	{
		mScene = scene;
	}

	Entity SceneOverviewWindow::AddItem(const Entity& parent)
	{
		if (mScene->HasComponent<Heirarchy>(parent))
		{

			Entity e{ mScene->CreateEntity() };

			Heirarchy& ph{ mScene->GetComponent<Heirarchy>(parent) };
			ph.Children.push_back(e);

			mScene->Attach<Description>(e, {"Entity"});
			mScene->Attach<Transform>(e);
			mScene->Attach<Sprite>(e);
			mScene->Attach<Heirarchy>(e, parent);

			return e;
		}
		else
		{
			// No heirarchy component for parent
			std::quick_exit(EXIT_FAILURE);
		}
	}

	void SceneOverviewWindow::DrawEntityTree(const Entity& parent)
	{
		if (mScene->HasComponent<Heirarchy>(parent))
		{
			Heirarchy& h{ mScene->GetComponent<Heirarchy>(parent) };
			if (h.Children.size() > 0)
			{
				for (int32_t b{ 0 }; b < h.Children.size();)
				{
					bool isDeleted{ false };

					std::vector<Entity>::iterator it{ h.Children.begin() + b };
					Entity& i{ *it };
					if (mScene->HasComponent<Description>(i))
					{
						ImGuiTreeNodeFlags flags{ ImGuiTreeNodeFlags_OpenOnArrow | ((mEntity == i) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen };

						Description& d{ mScene->GetComponent<Description>(i) };
						bool open{ ImGui::TreeNodeEx((void*)i, flags, d.Name.c_str()) };

						if (ImGui::IsItemClicked())
						{
							mEntity = i;
						}

						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::Button("Add Child"))
							{
								Entity newID{ AddItem(i) };
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::Button("Delete"))
							{
								Heirarchy& h2{ mScene->GetComponent<Heirarchy>(i) };
								if (h2.Children.size() > 0)
								{
									std::vector<Entity> children{ h2.Children };
									for (Entity& i2 : children)
									{
										mScene->DestroyEntity(i2);
									}
								}

								mScene->DestroyEntity(i);
								mEntity = nil;

								isDeleted = true;

								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}

						if (open)
						{
							DrawEntityTree(i);
							ImGui::TreePop();
						}

					}

					if (isDeleted)
					{
						h.Children.erase(it);
					}
					else
					{
						b++;
					}
				}

				/*
				for (std::vector<Entity>::iterator it{ h.Children.begin() }; it < h.Children.end();)
				{
					bool isDeleted{ false };
					Entity& i{ *it };
					if (mScene->HasComponent<Description>(i))
					{
						ImGuiTreeNodeFlags flags{ ImGuiTreeNodeFlags_OpenOnArrow | ((mEntity == i) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanAvailWidth };

						Description& d{ mScene->GetComponent<Description>(i) };
						bool open{ ImGui::TreeNodeEx((void*)i, flags, d.Name.c_str()) };

						if (ImGui::IsItemClicked())
						{
							mEntity = i;
						}

						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::Button("Add Child"))
							{
								Entity newID{ AddItem(i) };
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::Button("Delete"))
							{
								Heirarchy& h2{ mScene->GetComponent<Heirarchy>(i) };
								for (Entity& i2 : h2.Children)
								{
									mScene->DestroyEntity(i2);
								}

								mScene->DestroyEntity(i);
								mEntity = nil;

								isDeleted = true;

								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}

						if (open)
						{
							DrawEntityTree(i);
							ImGui::TreePop();
						}

					}

					if (isDeleted)
					{
						h.Children.erase(it);
					}
					else
					{
						it++;
					}

				}
				*/
			}
		}
	}

}