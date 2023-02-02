#pragma once

#include "axt/Core.h"

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace axt {

	class AXT_API Render2D {
	public:
		static void Init();
		static void Shutdown();
		static void SceneStart(const OrthoCamera& camera);
		static void SceneEnd();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const float& rotation = 0.f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const float& rotation = 0.f);
	};

}
