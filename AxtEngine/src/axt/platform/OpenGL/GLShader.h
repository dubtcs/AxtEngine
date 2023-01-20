#pragma once

#include "axt/Core.h"

#include "axt/render/Shader.h"

#include <glm/glm.hpp>
#include <string>

namespace axt {

	class AXT_API GLShader : public Shader {
	public:
		GLShader(const std::string vertexSource, const std::string fragmentSource);
		virtual ~GLShader() override;
	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
	public:
		//void SetUniform(const std::string& name, const glm::mat4& uniform) const;
		void SetValue(const std::string& name, const glm::vec4& vec4) const;
		void SetValue(const std::string& name, const glm::vec3& vec3) const;
		void SetValue(const std::string& name, const glm::vec2& vec2) const;
		void SetValue(const std::string& name, const glm::mat3& mat3) const;
		void SetValue(const std::string& name, const glm::mat4& mat4) const;
	private:
		uint32_t id;
	};

}
