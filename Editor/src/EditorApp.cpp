
#include "EditorApp.h"

#include <axt/EntryPoint.h>

#include "layers/ELayer.h"

#include <axt/ecs/nECS.h>
#include <necs/include.h>
#include <entt/entt.hpp>

#include <glm/gtx/string_cast.hpp>

namespace axt
{

	struct bruh
	{
		glm::vec3 Position;
	};

	App* axt::CreateApp()
	{

		//AXT_PROFILE_FUNCTION();

		{
			AXT_PROFILE_SCOPE("a entt");

			using namespace entt;

			registry s{};

			std::vector<entity> mIds{};

			for (int32_t i{ 0 }; i < 5; i++)
				mIds.push_back(s.create());

			for (entity i : mIds)
				s.emplace<bruh>(i);

			for (entity i : mIds)
			{
				bruh& b{ s.get<bruh>(i) };
				b.Position = { 1.9f, 2.2f, 1.5f };
			}

			for (entity i : mIds)
			{
				bruh& b{ s.get<bruh>(i) };
				std::cout << glm::to_string(b.Position) << "\n";
			}

		}

		{
			AXT_PROFILE_SCOPE("b nECS3");

			using namespace necs;

			Scene s{};

			std::vector<Entity> mIds{};

			for (int32_t i{ 0 }; i < 5; i++)
				mIds.push_back(s.CreateEntity());

			for (Entity i : mIds)
				s.Attach<bruh>(i);

			for (Entity i : mIds)
			{
				bruh& b{ s.GetComponent<bruh>(i) };
				b.Position = { 1.9f, 2.2f, 1.5f };
			}

			for (Entity i : mIds)
			{
				bruh& b{ s.GetComponent<bruh>(i) };
				std::cout << glm::to_string(b.Position) << "\n";
			}

		}

		{
			AXT_PROFILE_SCOPE("c nECS_char");

			using namespace axt::ecs;

			Scene s{};

			std::vector<EntityID> mIds{};

			for (int32_t i{ 0 }; i < 5; i++)
				mIds.push_back(s.CreateEntity());

			for (EntityID i : mIds)
				s.Attach<bruh>(i);

			for (EntityID i : mIds)
			{
				bruh& b{ s.GetComponent<bruh>(i) };
				b.Position = { 1.9f, 2.2f, 1.5f };
			}

			for (EntityID i : mIds)
			{
				bruh& b{ s.GetComponent<bruh>(i) };
				std::cout << glm::to_string(b.Position) << "\n";
			}

		}

		return new Editor{};
	}

	Editor::Editor() : App{}
	{
		PushLayer(new ELayer{});
	}

	Editor::~Editor()
	{

	}

}
