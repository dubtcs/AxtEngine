#pragma once

#include "axt/Core.h"

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

#include "axt/render/Texture.h"

namespace axt {

	class AXT_API Render2D {
	public:
		struct AXT_API QuadProperties {
			glm::vec3 position{ 1.f };
			glm::vec2 size{ 1.f };
			glm::vec4 color{ 1.f };
			std::string texName = "White";
			float rotation = 0.f;
		};
	public:
		static void Init();
		static void Shutdown();
		static void SceneStart(const OrthoCamera& camera);
		static void SceneEnd();
		static void Flush();

		static void DrawQuad(const QuadProperties&& props);
	};

}
