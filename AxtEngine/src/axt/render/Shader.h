#pragma once

#include <string>

#include "axt/Core.h"

namespace axt {

	class AXT_API Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();
	public:
		void Bind() const;
		static void Unbind();
	private:
		unsigned int ID;
	};

}
