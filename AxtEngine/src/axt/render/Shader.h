#pragma once

#include "axt/Core.h"

#include <string>

namespace axt {

	enum ShaderType {
		Vertex = 1 << 0,
		Pixel = 1 << 1,
		Geometry = 1 << 2,
		/*
		Tesselation = 1 <, 3;
		Mesh = 1 << 4;
		Raytrace = 1 << 5;
		*/
	};

	class AXT_API Shader {
	public:
		virtual ~Shader() {}
		static Ref<Shader> Create(const std::string& name, const std::string& filepath, unsigned int shadertypes);
		static Ref<Shader> Create(const std::string vertexSource, const std::string pixelSource); // deprecated

		//static Ref<Shader> Create(const std::string& filepath);
		/*template<typename T>
		static T* GetPlatformShader(Shader*& bruh) {
			return std::static_pointer_cast<T>(bruh);
		};*/
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const std::string& GetName() const = 0;
		//virtual void SetUniform(const std::string& name, const glm::mat4& uniform) const = 0;
	};

	class AXT_API ShaderLib {
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Get(const std::string& name) const;
		Ref<Shader> Load(const std::string& filepath); //  need to manually set shaders flags. Make a new way to do this automatically
		Ref<Shader> Load(const std::string& filepath, const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> myShaderMap;
	};

}
