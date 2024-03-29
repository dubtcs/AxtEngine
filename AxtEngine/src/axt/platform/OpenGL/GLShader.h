#pragma once

#include "axt/Core.h"

#include "axt/render/Shader.h"

#include <glm/glm.hpp>
#include <string>

namespace axt {

	class AXT_API GLShader : public Shader {
	public:
		GLShader(const std::string& filepath);
		GLShader(const std::string& name, const std::string& filepath, unsigned int shadertypes);
		GLShader(const std::string vertexSource, const std::string fragmentSource); // deprecated
		virtual ~GLShader() override;
	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual const std::string& GetName() const override;
	public:
		//void SetUniform(const std::string& name, const glm::mat4& uniform) const;
		virtual void SetValue(const std::string& name, const glm::vec2& vec2) const override;
		virtual void SetValue(const std::string& name, const glm::vec3& vec3) const override;
		virtual void SetValue(const std::string& name, const glm::vec4& vec4) const override;

		virtual void SetValue(const std::string& name, const glm::mat3& mat3) const override;
		virtual void SetValue(const std::string& name, const glm::mat4& mat4) const override;

		virtual void SetValue(const std::string& name, const int& val) const override;

		virtual void SetValue(const std::string& name, int* start, const int count) const override;
	private:
		std::string myName;
		uint32_t id;
	};

}
