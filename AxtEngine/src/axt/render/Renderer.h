#pragma once

#include "axt/Core.h"

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace axt {

	class AXT_API Render3D {
	public:
		static void Init();
		static void SceneStart(const OrthoCamera& ortho);
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& modelTransform = glm::mat4{ 1.f });
		static void SceneEnd();
		static void WindowResized(const uint32_t x, const uint32_t y);
		static RenderAPI::API GetApi() { return RenderAPI::GetAPI(); };
	private:
		struct SceneData {
			glm::mat4 viewProjection;
		};
		static SceneData* scene;
	};

	class AXT_API Render2D {
	public:
		//static void Init();
		//static void WindowResized(const uint32_t x, const uint32_t y);
	};

}
