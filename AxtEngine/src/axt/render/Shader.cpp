#include <pch.h>

#include "Shader.h"

#include "Renderer.h"
#include "axt/platform/OpenGL/GLShader.h"

namespace axt {

	/**********/
	/* SHADER */
	/**********/

	Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath, unsigned int shadertypes) {
		switch (Render3D::GetApi()) {
		case(RenderAPI::API::None): break;
		case(RenderAPI::API::OpenGL): return NewRef<GLShader>(name, filepath, shadertypes);
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

	// deprecated
	Ref<Shader> Shader::Create(const std::string vertexSource, const std::string pixelSource) {
		switch (Render3D::GetApi()) {
		case(RenderAPI::API::None): break;
		case(RenderAPI::API::OpenGL): return NewRef<GLShader>(vertexSource, pixelSource);//return (new GLShader{vertexSource, pixelSource});
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

	/******************/
	/* SHADER LIBRARY */
	/******************/

	void ShaderLib::Add(const Ref<Shader>& shader) {
		const std::string& shaderName{ shader->GetName() };
		AXT_CORE_ASSERT((myShaderMap.find(shaderName) == myShaderMap.end()), "Duplicate shader name found.");
		myShaderMap[shaderName] = shader;
	}

	void ShaderLib::Add(const std::string& name, const Ref<Shader>& shader) {
		AXT_CORE_ASSERT((myShaderMap.find(name) == myShaderMap.end()), "Duplicate shader name found.");
		myShaderMap[name] = shader;
	}

	Ref<Shader> ShaderLib::Load(const std::string& name, const std::string& filepath) {
		const Ref<Shader> newShader{ Shader::Create(name, filepath, ShaderType::Vertex & ShaderType::Pixel)};
		return newShader;
	}

	Ref<Shader> ShaderLib::Get(const std::string& name) const {
		AXT_CORE_ASSERT((myShaderMap.find(name) != myShaderMap.end()), "Shader not found.");
		return (myShaderMap.at(name));
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