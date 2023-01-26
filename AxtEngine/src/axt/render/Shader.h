#pragma once

#include "axt/Core.h"

#include <string>

namespace axt {

	enum ShaderType {
		Vertex = 1 << 0,
		Pixel = 1 << 1,
		Geometry = 1 << 2,
	};

	class AXT_API Shader {
	public:
		virtual ~Shader() {}
		static Ref<Shader> Create(const std::string& filepath, unsigned int shadertypes);
		//static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string vertexSource, const std::string pixelSource); // deprecated

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
