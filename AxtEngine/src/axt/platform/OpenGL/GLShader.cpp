#include <pch.h>

#include "GLShader.h"

#include "axt/core/OpenShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

static const char* GLSL_VERSION{"#version 450\n"};
static const unsigned short MAX_SHADER_CAPACITY{ 3 };

namespace axt {

	GLShader::GLShader(const std::string& filepath) {

	}

	GLShader::GLShader(const std::string& name, const std::string& filepath, unsigned int shadertypes) : myName{ name } {
		//AXT_CORE_TRACE(shadertypes);
		char info[512]; // for debug
		std::string shaderSource{ OpenShader(filepath) };
		std::array<unsigned int, MAX_SHADER_CAPACITY> shaderIds;
		unsigned int currentShaderIndex{ 0 };

		// vertex
		if (shadertypes & ShaderType::Vertex) {
			const char* src[3]{ GLSL_VERSION, "#define GLSL_VERTEX\n", shaderSource.c_str() };
			unsigned int vid{ glCreateShader(GL_VERTEX_SHADER) };
			shaderIds[currentShaderIndex++] = vid;

			glShaderSource(vid, 3, src, 0);
			glCompileShader(vid);
			int pass{ 0 };
			glGetShaderiv(vid, GL_COMPILE_STATUS, &pass);
			if (pass == GL_FALSE) {
				glGetShaderInfoLog(vid, 512, nullptr, info);
				for (unsigned int i{ 0 }; i < currentShaderIndex; i++) {
					glDeleteShader(shaderIds[i]);
				}
				AXT_CORE_ERROR("Vertex shader error: {0}", info);
				return;
			}
		}
		// pixel / fragment
		if (shadertypes & ShaderType::Pixel) {
			const char* src[3]{ GLSL_VERSION, "#define GLSL_FRAGMENT\n", shaderSource.c_str() };
			unsigned int fid{ glCreateShader(GL_FRAGMENT_SHADER) };
			shaderIds[currentShaderIndex++] = fid;

			glShaderSource(fid, 3, src, 0);
			glCompileShader(fid);
			int pass{ 0 };
			glGetShaderiv(fid, GL_COMPILE_STATUS, &pass);
			if (pass == GL_FALSE) {
				glGetShaderInfoLog(fid, 512, nullptr, info);
				for (unsigned int i{ 0 }; i < currentShaderIndex; i++) {
					glDeleteShader(shaderIds[i]);
				}
				AXT_CORE_ERROR("Fragment shader error: {0}", info);
				return;
			}
		}

		// Shader Program
		id = glCreateProgram();
		int pass{ 0 };
		//AXT_CORE_TRACE("Shader type total: {0}", currentShaderIndex);

		for (unsigned int i{ 0 }; i <= currentShaderIndex; i++) {
			glAttachShader(id, shaderIds[i]);
		}
		glLinkProgram(id);
		glGetProgramiv(id, GL_LINK_STATUS, (int*)&pass);
		if (pass == GL_FALSE) {
			glGetProgramInfoLog(id, 512, nullptr, info);
			for (unsigned int i{ 0 }; i <= currentShaderIndex; i++) {
				glDeleteShader(shaderIds[i]);
			}
			glDeleteProgram(id);
			AXT_CORE_ERROR("Shader program linking error: {0}", info);
			return;
		}

		for (unsigned int i{ 0 }; i <= currentShaderIndex; i++) {
			glDetachShader(id, shaderIds[i]);
		}

	}

	GLShader::GLShader(const std::string vSource, const std::string fSource) {
		char info[512];
		// Vertex Shader
		unsigned int vid{ glCreateShader(GL_VERTEX_SHADER) };
		const char* src{ vSource.c_str() };
		glShaderSource(vid, 1, &src, 0);
		glCompileShader(vid);
		int pass{ 0 };
		glGetShaderiv(vid, GL_COMPILE_STATUS, &pass);
		if (pass == GL_FALSE) {
			glGetShaderInfoLog(vid, 512, nullptr, info);
			glDeleteShader(vid);
			AXT_CORE_ERROR("Vertex shader error: {0}", info);
			return;
		}

		// Fragment Shader
		unsigned int fid{ glCreateShader(GL_FRAGMENT_SHADER) };
		src = fSource.c_str();
		glShaderSource(fid, 1, &src, 0);
		glCompileShader(fid);
		glGetShaderiv(fid, GL_COMPILE_STATUS, &pass);
		if (pass == GL_FALSE) {
			glGetShaderInfoLog(fid, 512, nullptr, info);
			glDeleteShader(vid);
			glDeleteShader(fid);
			AXT_CORE_ERROR("Fragment shader error: {0}", info);
			return;
		}

		// Shader Program
		id = glCreateProgram();
		glAttachShader(id, vid);
		glAttachShader(id, fid);
		glLinkProgram(id);
		glGetProgramiv(id, GL_LINK_STATUS, (int*)&pass);
		if (pass == GL_FALSE) {
			glGetProgramInfoLog(id, 512, nullptr, info);
			glDeleteShader(vid);
			glDeleteShader(fid);
			glDeleteProgram(id);
			AXT_CORE_ERROR("Shader program linking error: {0}", info);
			return;
		}

		glDetachShader(id, fid);
		glDetachShader(id, fid);
	}

	GLShader::~GLShader() {

	}
	void GLShader::Bind() const {
		glUseProgram(id);
	}

	void GLShader::Unbind() const {
		glUseProgram(0);
	}

	const std::string& GLShader::GetName() const {
		return myName;
	}

	/*void GLShader::SetUniform(const std::string& name, const glm::mat4& uniform) const {
		GLint loc{ glGetUniformLocation(id, name.c_str()) };
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(uniform));
	}*/

	void GLShader::SetValue(const std::string& name, const glm::mat4& mat4) const {
		GLint loc{ glGetUniformLocation(id, name.c_str()) };
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4));
	}
	void GLShader::SetValue(const std::string& name, const glm::mat3& mat3) const {
		GLint loc{ glGetUniformLocation(id, name.c_str()) };
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat3));
	}
	void GLShader::SetValue(const std::string& name, const glm::vec4& vec4) const {
		GLint loc{ glGetUniformLocation(id, name.c_str()) };
		glUniform4f(loc, vec4.x, vec4.y, vec4.z, vec4.w);
	}
	void GLShader::SetValue(const std::string& name, const glm::vec3& vec3) const {
		GLint loc{ glGetUniformLocation(id, name.c_str()) };
		glUniform3f(loc, vec3.x, vec3.y, vec3.z);
	}
	void GLShader::SetValue(const std::string& name, const glm::vec2& vec2) const {
		GLint loc{ glGetUniformLocation(id, name.c_str()) };
		glUniform2f(loc, vec2.x, vec2.y);
	}
	void GLShader::SetValue(const std::string& name, const int& val) const {
		GLint loc{ glGetUniformLocation(id, name.c_str()) };
		glUniform1i(loc, val);
	}
}