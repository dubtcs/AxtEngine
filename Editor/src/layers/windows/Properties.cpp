#include <pch.h>

#include "Properties.h"

#include <axt/world/Components.h>
#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace axt
{
	using namespace ecs;

	PropertiesPanel::PropertiesPanel(const Ref<World>& scene) :
		mWorld{ scene }
	{
		// hehe
	}

	void PropertiesPanel::SetWorld(const Ref<World>& scene)
	{
		mWorld = scene;
	}

	void PropertiesPanel::OnImGuiRender(const EntityID& id)
	{
		ImGui::Begin("Properties");

		if (id < gMaxEntitiesOOB)
		{
			if (mWorld->HasComponent<Description>(id))
			{
				Description& d{ mWorld->GetComponent<Description>(id) };
				char buffer[256];
				//std::array<char, 256> buffer;
				strncpy_s(buffer, d.Name, sizeof(buffer));
				if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
					d.ChangeName(std::string{ buffer });
				}
			}

			if (mWorld->HasComponent<Position>(id))
			{
				Position& p{ mWorld->GetComponent<Position>(id) };
				ImGui::DragFloat3("Position", glm::value_ptr(p.Value), 0.05f);
			}

			if (mWorld->HasComponent<Color>(id))
			{
				Color& c{ mWorld->GetComponent<Color>(id) };
				ImGui::ColorEdit4("Color", glm::value_ptr(c.Value));
			}

		}

		ImGui::End();
	}

}