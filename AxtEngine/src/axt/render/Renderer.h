#pragma once

// contains both 2D and 3D render systems
#include "Render2D.h"
#include "Render3D.h"

namespace axt {

	namespace Renderer {
		static void Init() { RenderCommand::Init(); }
		static void WindowResize(const uint32_t x, const uint32_t y) { RenderCommand::SetViewport(0, 0, x, y); }
		inline static RenderAPI::API GetApi() { return RenderAPI::GetAPI(); }

	}

}
