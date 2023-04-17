
#include "EditorApp.h"

#include <axt/EntryPoint.h>

#include "layers/ELayer.h"

#include <necs/include.h>
//#include <entt/entt.hpp>

#include <glm/gtx/string_cast.hpp>

#include <axt/world/components/all.h>

namespace axt
{

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
