#pragma once

#include "axt/Core.h"

namespace axt {

	enum class RenderAPI {
		None = 0, OpenGL, //DirectX, Vulkan
	};

	class AXT_API Renderer {
	public:
		static RenderAPI GetApi();
		static RenderAPI api;
	};

}
