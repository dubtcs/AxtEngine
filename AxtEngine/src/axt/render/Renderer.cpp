#include <pch.h>

#include "Renderer.h"

namespace axt {

	RenderAPI Renderer::api{ RenderAPI::OpenGL };

	RenderAPI Renderer::GetApi() {
		return api;
	}

}