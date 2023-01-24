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
	void GLShader::Bind() const {
		glUseProgram(id);
	}

	void GLShader::Unbind() const {
		glUseProgram(0);
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