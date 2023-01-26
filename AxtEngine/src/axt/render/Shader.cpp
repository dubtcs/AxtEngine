#include <pch.h>

#include "Shader.h"

#include "Renderer.h"
#include "axt/platform/OpenGL/GLShader.h"

namespace axt {

	Ref<Shader> Shader::Create(const std::string& filepath, unsigned int shadertypes) {
		switch (Renderer::GetApi()) {
		case(RenderAPI::API::None): break;
		case(RenderAPI::API::OpenGL): return std::make_shared<GLShader>(filepath, shadertypes);
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

	// deprecated
	Ref<Shader> Shader::Create(const std::string vertexSource, const std::string pixelSource) {
		switch (Renderer::GetApi()) {
		case(RenderAPI::API::None): break;
		case(RenderAPI::API::OpenGL): return std::make_shared<GLShader>(vertexSource, pixelSource);//return (new GLShader{vertexSource, pixelSource});
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

	/*
	Ref<Shader> Shader::Create(const std::string& filepath) {
		switch (Renderer::GetApi()) {
		case(RenderAPI::API::None): break;
		case(RenderAPI::API::OpenGL): return std::make_shared<GLShader>(filepath);
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}
	*/

}