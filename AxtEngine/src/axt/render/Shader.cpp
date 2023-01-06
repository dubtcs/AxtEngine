#include <pch.h>

#include "Shader.h"

#include "Renderer.h"
#include "axt/platform/OpenGL/GLShader.h"

namespace axt {

	Shader* Shader::Create(const std::string vertexSource, const std::string pixelSource) {
		switch (Renderer::GetApi()) {
		case(RenderAPI::None): break;
		case(RenderAPI::OpenGL): return (new GLShader{vertexSource, pixelSource});
		}

		assert(false); // no api found
		return nullptr;
	}

}