#pragma once

#include "axt/Core.h"

#include "RenderCommand.h"
#include "OrthoCamera.h"
#include "Shader.h"

#include <glm/glm.hpp>

namespace axt {

	class AXT_API Render3D {
	public:
		struct CubeProperties
		{
			glm::vec3 Position{ 0.f };
			glm::vec3 Rotation{ 0.f };
			glm::vec3 Scale{ 1.f };
			glm::vec4 Color{ 1.f };
		};
	public:
		static void Init();
		static void Shutdown();
		static void SceneStart(const glm::mat4& viewProjection);
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& modelTransform = glm::mat4{ 1.f });
		static void SceneEnd();
		static void Stage();
		static void DrawCube(const CubeProperties& properties);
		static RenderAPI::API GetApi() { return RenderAPI::GetAPI(); };
	};

}
