#pragma once

#include "axt/Core.h"

#include <string>

namespace axt {

	class AXT_API Shader {
	public:
		virtual ~Shader() {}
		static Shader* Create(const std::string vertexSource, const std::string pixelSource);

		/*template<typename T>
		static T* GetPlatformShader(Shader*& bruh) {
			return std::static_pointer_cast<T>(bruh);
		};*/
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		//virtual void SetUniform(const std::string& name, const glm::mat4& uniform) const = 0;
	};

}
