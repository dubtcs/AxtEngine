#pragma once

#include "axt/Core.h"

#include "RenderCommand.h"
#include "OrthoCamera.h"
#include "Shader.h"

#include <axt/world/components/Camera.h>

#include "axt/render/Texture.h"

#include <necs/etypes.h>

namespace axt 
{

	class AXT_API Render2D 
	{
	public:
		struct RenderStats 
		{
			int drawCalls{ 0 };
			int quads{ 0 };
			int textures{ 0 };
		};
		struct AXT_API QuadProperties 
		{
			glm::vec3 position{ 0.f };
			glm::vec3 rotation{ 0.f };
			glm::vec3 size{ 1.f };
			glm::vec4 color{ 1.f };
			std::string texName{ "White" };
			float textureTiling{ 1.f };

			// only really used in editor
			// use bounding box selection later on
			necs::Entity EntityId{ necs::nil };
		};
	public:
		static void Init();
		static void Shutdown();
		static void SceneStart(const OrthoCamera& camera);
		static void SceneStart(const glm::mat4& viewProjection);
		static void SceneStart(const Camera& camera, const glm::mat4& viewProjection);
		static void SceneEnd();

		// draws data loaded on gpu
		static void Stage();

		static void DrawQuad(const QuadProperties&& props);
		static void DrawCube(const QuadProperties&& props);

		static RenderStats GetStats();

	private:
		static void StageForOverflow();
	};

}
