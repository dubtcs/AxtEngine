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
	void DrawVec2Edit(const std::string& title, glm::vec3& vector);
	void DrawVec3Edit(const std::string& title, glm::vec3& vector);
	void DrawVec4Edit(const std::string& title, glm::vec4& vector);
	void DrawTitle(const std::string& title);

	void PropertiesWindow::OnImGuiRender(Ref<GameWorld>& world, const Entity& id)
	{
		//ImGui::ShowDemoWindow();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 6.f, 4.f });
		ImGui::Begin("Properties");

		if (id != necs::nil)
		{
			ImGuiIO& io{ ImGui::GetIO() };
			if (world->HasComponent<Description>(id))
			{
				DrawTitle("Description");

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
				DrawTitle("Transform");

				ImGui::SameLine();
				ImGui::Button("Remove");

				Transform& t{ world->GetComponent<Transform>(id) };
				DrawVec3Edit("Position", t.Position);
				DrawVec3Edit("Rotation", t.Rotation);
				DrawVec3Edit("Scale", t.Scale);
				ImGui::Separator();
			}

			if (world->HasComponent<Sprite>(id))
			{
				DrawTitle("Sprite");

				Sprite& sp{ world->GetComponent<Sprite>(id) };
				DrawVec4Edit("Color", sp.Color);
				ImGui::Separator();
			}
			if (world->HasComponent<Mesh>(id))
			{
				DrawTitle("Mesh");

				Mesh& sp{ world->GetComponent<Mesh>(id) };

				ImGui::Text("Mesh Path:");
				DrawVec4Edit("Color", sp.Color);
				ImGui::Separator();
			}

			if (world->HasComponent<Camera>(id))
			{
				DrawTitle("Camera");

				Camera& cam{ world->GetComponent<Camera>(id) };
				ImGui::Text("Aspect Ratio: %f", cam.AspectRatio);
				ImGui::Text("Zoom");
				ImGui::SameLine();
				ImGui::DragFloat("##Zoom", &cam.Zoom, 0.1f, 0.25f, 25.f);
				ImGui::Separator();
			}

		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void DrawTitle(const std::string& title)
	{
		ImGuiIO& io{ ImGui::GetIO() };
		ImGui::PushFont(io.Fonts->Fonts[1]);
		ImGui::Text(title.c_str());
		ImGui::PopFont();
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

	void DrawVec2Edit(const std::string& title, glm::vec3& vector)
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

	void DrawVec4Edit(const std::string& title, glm::vec4& vector)
	{
		ImGuiIO& io{ ImGui::GetIO() };
		ImFont*& bold{ io.Fonts->Fonts[1] };

		ImGuiTableFlags flags{ ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadInnerX };

		ImGui::BeginTable(title.c_str(), 6, flags);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(title.c_str());

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f,0.f });

		ImGui::TableSetColumnIndex(1);
		ImGui::PushStyleColor(ImGuiCol_Button, gSalmon);
		ImGui::PushFont(bold);
		if (ImGui::Button("R"))
		{
			vector.x = 0.f;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::InputFloat("##R", &vector.x);

		ImGui::TableSetColumnIndex(2);
		ImGui::PushStyleColor(ImGuiCol_Button, gGreen);
		ImGui::PushFont(bold);
		if (ImGui::Button("G"))
		{
			vector.y = 0.f;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::InputFloat("##G", &vector.y);

		ImGui::TableSetColumnIndex(3);
		ImGui::PushStyleColor(ImGuiCol_Button, gPurple);
		ImGui::PushFont(bold);
		if (ImGui::Button("B"))
		{
			vector.z = 0.f;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::InputFloat("##B", &vector.z);

		ImGui::TableSetColumnIndex(4);
		ImGui::PushStyleColor(ImGuiCol_Button, gPurple);
		ImGui::PushFont(bold);
		if (ImGui::Button("A"))
		{
			vector.w = 0.f;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::InputFloat("##A", &vector.w);

		ImGui::TableSetColumnIndex(5);
		std::string format{ std::format("##%s", title) };
		ImGui::ColorEdit4(format.c_str(), glm::value_ptr(vector), ImGuiColorEditFlags_NoInputs);

		ImGui::PopStyleVar();
		ImGui::EndTable();

	}

}
