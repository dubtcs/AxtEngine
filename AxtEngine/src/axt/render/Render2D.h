#pragma once

#include "axt/Core.h"

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

#include "axt/render/Texture.h"

namespace axt {

	class AXT_API Render2D {
	public:
		struct RenderStats {
			int drawCalls{ 0 };
			int quads{ 0 };
			int textures{ 0 };
		};
		struct AXT_API QuadProperties {
			glm::vec3 position{ 1.f };
			glm::vec2 size{ 1.f };
			glm::vec4 color{ 1.f };
			std::string texName{ "White" };
			float rotation{ 0.f };
			float textureTiling{ 1.f };
		};
	public:
		static void Init();
		static void Shutdown();
		static void SceneStart(const OrthoCamera& camera);
		static void SceneEnd();

		// draws data loaded on gpu
		static void Stage();

		static void DrawQuad(const QuadProperties&& props);

		static RenderStats GetStats();

	private:
		static void StageForOverflow();
	};

}
