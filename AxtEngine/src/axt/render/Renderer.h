#pragma once

#include "axt/Core.h"

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace axt {

	class AXT_API Renderer {
	public:
		static void SceneStart(const OrthoCamera& ortho);
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);
		static void SceneEnd();
		static RenderAPI::API GetApi() { return RenderAPI::GetAPI(); };
	private:
		struct SceneData {
			glm::mat4 viewProjection;
		};
		static SceneData* scene;
	};

}
