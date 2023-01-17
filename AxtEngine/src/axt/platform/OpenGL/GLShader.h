#pragma once

#include "axt/Core.h"

#include "axt/render/Shader.h"

#include <string>

namespace axt {

	class AXT_API GLShader : public Shader {
	public:
		GLShader(const std::string vertexSource, const std::string fragmentSource);
		virtual ~GLShader() override;
	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetUniform(const std::string& name, const glm::mat4& uniform) const override;
	private:
		uint32_t id;
	};

}
