#include <pch.h>

#include "RenderCommand.h"

//temp
#include "axt/platform/OpenGL/GLRenderAPI.h"

namespace axt {

	RenderAPI* RenderCommand::renderApi{ new GLRenderAPI{} };



}