#include <pch.h>

#include "PropertiesWindow.h"

#include <axt/world/components/all.h>
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

static const ImVec4 gGreen{ 0.04f, .91f, .51f, 1.f };
static const ImVec4 gPurple{ 0.34f, 0.37f, 0.81f, 1.f };
static const ImVec4 gSalmon{ 0.94f, 0.34f, .47f, 1.f };

namespace axt
{

	using namespace necs;

	void DrawVec2Edit(const std::string& title, glm::vec2& vector);
	void DrawVec3Edit(const std::string& title, glm::vec3& vector);

	void PropertiesWindow::OnImGuiRender(Ref<GameWorld>& world, const Entity& id)
	{
		//ImGui::ShowDemoWindow();

		ImGui::Begin("Properties");

		if (id != necs::nil)
		{
			if (world->HasComponent<Description>(id))
			{
				ImGui::Text("Description");
				ImGui::Text("Name: ");
				Description& d{ world->GetComponent<Description>(id) };
				char buffer[256];
				strcpy_s(buffer, sizeof(buffer), d.Name.c_str());
				ImGui::SameLine();
				if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
				{
					d.Name = std::string{ buffer };
				}

				ImGui::Separator();
			}

			if (world->HasComponent<Transform>(id))
			{
				ImGui::Text("Transform");
				Transform& t{ world->GetComponent<Transform>(id) };
				DrawVec3Edit("Position", t.Position);
				DrawVec3Edit("Rotation", t.Rotation);
				ImGui::Separator();
			}

			if (world->HasComponent<Sprite>(id))
			{
				ImGui::Text("Sprite");
				Sprite& sp{ world->GetComponent<Sprite>(id) };
				ImGui::Text("Color");
				ImGui::SameLine();
				ImGui::ColorEdit4("##Color", glm::value_ptr(sp.Color), 0.1f);
				DrawVec2Edit("Size", sp.Size);
				ImGui::Separator();
			}

		}

		ImGui::End();
	}

	void DrawVec2Edit(const std::string& title, glm::vec2& vector)
	{

		ImGuiIO& io{ ImGui::GetIO() };
		ImFont*& bold{ io.Fonts->Fonts[1] };

		ImGuiTableFlags flags{ ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadInnerX };

		ImGui::BeginTable(title.c_str(), 3, flags);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(title.c_str());

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });

		ImGui::TableSetColumnIndex(1);
		ImGui::PushStyleColor(ImGuiCol_Button, gSalmon);
		ImGui::PushFont(bold);
		if (ImGui::Button("X"))
		{
			vector.x = 1.f;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::InputFloat("##X", &vector.x);

		ImGui::TableSetColumnIndex(2);
		ImGui::PushStyleColor(ImGuiCol_Button, gGreen);
		ImGui::PushFont(bold);
		if (ImGui::Button("Y"))
		{
			vector.y = 1.f;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::InputFloat("##Y", &vector.y);

		ImGui::PopStyleVar();
		ImGui::EndTable();
	}

	void DrawVec3Edit(const std::string& title, glm::vec3& vector)
	{

		ImGuiIO& io{ ImGui::GetIO() };
		ImFont*& bold{ io.Fonts->Fonts[1] };

		ImGuiTableFlags flags{ ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadInnerX };

		ImGui::BeginTable(title.c_str(), 4, flags);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(title.c_str());

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f,0.f });

		ImGui::TableSetColumnIndex(1);
		ImGui::PushStyleColor(ImGuiCol_Button, gSalmon);
		ImGui::PushFont(bold);
		if (ImGui::Button("X"))
		{
			vector.x = 0.f;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::InputFloat("##X", &vector.x);

		ImGui::TableSetColumnIndex(2);
		ImGui::PushStyleColor(ImGuiCol_Button, gGreen);
		ImGui::PushFont(bold);
		if (ImGui::Button("Y"))
		{
			vector.y = 0.f;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::InputFloat("##Y", &vector.y);

		ImGui::TableSetColumnIndex(3);
		ImGui::PushStyleColor(ImGuiCol_Button, gPurple);
		ImGui::PushFont(bold);
		if (ImGui::Button("Z"))
		{
			vector.z = 0.f;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::InputFloat("##Z", &vector.z);

		ImGui::PopStyleVar();
		ImGui::EndTable();

	}

}
