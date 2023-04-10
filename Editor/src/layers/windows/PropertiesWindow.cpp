#include <pch.h>

#include "PropertiesWindow.h"

#include <axt/world/components/all.h>
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace axt
{

	using namespace necs;

	PropertiesWindow::PropertiesWindow(const Ref<Scene>& scene) :
		mScene{ scene }
	{

	}

	void PropertiesWindow::SetScene(const Ref<Scene>& scene)
	{
		mScene = scene;
	}

	void PropertiesWindow::OnImGuiRender(const Entity& id)
	{
		ImGui::Begin("Properties");

		if (id != necs::nil)
		{
			if (mScene->HasComponent<Description>(id))
			{
				ImGui::Text("Description");

				Description& d{ mScene->GetComponent<Description>(id) };
				char buffer[256];
				strcpy_s(buffer, sizeof(buffer), d.Name.c_str());
				if (ImGui::InputText("Name", buffer, sizeof(buffer)))
				{
					d.Name = std::string{ buffer };
				}

				ImGui::Separator();
			}

			if (mScene->HasComponent<Transform>(id))
			{
				ImGui::Text("Transform");
				Transform& t{ mScene->GetComponent<Transform>(id) };
				ImGui::DragFloat3("Position", glm::value_ptr(t.Position), 0.05f);
				ImGui::DragFloat3("Rotation", glm::value_ptr(t.Rotation), 0.05f);
				ImGui::Separator();
			}

			if (mScene->HasComponent<Sprite>(id))
			{
				ImGui::Text("Sprite");
				Sprite& sp{ mScene->GetComponent<Sprite>(id) };
				ImGui::ColorEdit4("Color", glm::value_ptr(sp.Color), 0.1f);
				ImGui::DragFloat2("Size", glm::value_ptr(sp.Size), 0.1f);
				ImGui::Separator();
			}

		}

		ImGui::End();
	}

	void PropertiesWindow::DrawEntityInfo(const Entity& id)
	{
		
	}

}
