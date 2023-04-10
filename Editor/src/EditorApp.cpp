
#include "EditorApp.h"

#include <axt/EntryPoint.h>

#include "layers/ELayer.h"

#include <axt/ecs/nECS.h>
#include <necs/include.h>
#include <entt/entt.hpp>

#include <glm/gtx/string_cast.hpp>

#include <axt/world/components/all.h>

namespace axt
{

	struct bruh
	{
		glm::vec3 Position;
	};

	App* axt::CreateApp()
	{
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
