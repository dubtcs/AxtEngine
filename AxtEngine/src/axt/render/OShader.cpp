#include <pch.h>

#include "Shader.h"

#include <glad/glad.h>

namespace axt {

	Shader::Shader(const std::string& vSrc, const std::string& fSrc) {

		// vertex
		unsigned int vShader{ glCreateShader(GL_VERTEX_SHADER) };
		const char* src{ vSrc.c_str() };
		glShaderSource(vShader, 1, &src, 0);
		glCompileShader(vShader);
		int suc{ 0 };
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &suc);
		if (suc == GL_FALSE) {
			int length{ 0 };
			glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> log(length);
			glGetShaderInfoLog(vShader, length, &length, &log[0]);
			glDeleteShader(vShader);

			AXT_CORE_ERROR("Vertex shader error: {0}", log.data());
			return;
		}

		// fragment (pixel)
		unsigned int fShader{ glCreateShader(GL_FRAGMENT_SHADER) };
		src = fSrc.c_str();
		glShaderSource(fShader, 1, &src, 0);
		glCompileShader(fShader);
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &suc);
		if (suc == GL_FALSE){
			int length{ 0 };
			glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> log(length);
			glGetShaderInfoLog(fShader, length, &length, &log[0]);
			glDeleteShader(fShader);
			glDeleteShader(vShader);

			AXT_CORE_ERROR("Fragment shader error: {0}", log.data());
			return;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vShader);
		glAttachShader(ID, fShader);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, (int*)&suc);
		if (suc == GL_FALSE) {
			int length{ 0 };
			glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> log(length);
			glGetShaderInfoLog(ID, length, &length, &log[0]);
			glDeleteProgram(ID);
			glDeleteShader(fShader);
			glDeleteShader(vShader);

			AXT_CORE_ERROR("Program compile error: {0}", log.data());
			return;
		}

		glDetachShader(ID, vShader);
		glDetachShader(ID, fShader);
	}

	Shader::~Shader() {

	}

	void Shader::Bind() const {
		glUseProgram(ID);
	}

	void Shader::Unbind() {
		glUseProgram(0);
	}

}