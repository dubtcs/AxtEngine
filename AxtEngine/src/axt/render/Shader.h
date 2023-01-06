#pragma once

#include "axt/Core.h"
#include <string>

namespace axt {

	class AXT_API Shader {
	public:
		virtual ~Shader() {}
		static Shader* Create(const std::string vertexSource, const std::string pixelSource);
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

}
