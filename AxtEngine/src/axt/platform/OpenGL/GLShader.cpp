#include <pch.h>

#include "GLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace axt {

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

	void GLShader::SetUniform(const std::string& name, const glm::mat4& uniform) const {
		GLint loc{ glGetUniformLocation(id, name.c_str()) };
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(uniform));
	}

	void GLShader::Bind() const {
		glUseProgram(id);
	}

	void GLShader::Unbind() const {
		glUseProgram(0);
	}

}