#include <pch.h>

#include "Texture.h"

#include "Renderer.h"
#include "axt/platform/OpenGL/GLTexture.h"

namespace axt {

	Ref<Texture2D> Texture2D::Create(const std::string& filepath) {
		switch (Render3D::GetApi()) {
		case(RenderAPI::API::None): break;
		case(RenderAPI::API::OpenGL): return std::make_shared<GLTexture2D>(filepath);
		}

		AXT_CORE_ASSERT(false, "No render api found for Texture2D");
		return nullptr;
	}

}