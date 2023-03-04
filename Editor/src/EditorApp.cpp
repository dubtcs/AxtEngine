
#include "EditorApp.h"

#include <axt/EntryPoint.h>

#include "layers/ELayer.h"

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
