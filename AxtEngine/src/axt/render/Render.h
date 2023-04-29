#pragma once

// 2D and 3D functionality

#include <axt/Core.h>

#include <glm/glm.hpp>

namespace axt
{

	namespace render
	{

		struct ObjectProperties
		{
			glm::vec3 Position;
			glm::vec3 Rotation;
			glm::vec3 Scale;
		};

		void Init();
		void SceneBegin();
		void SceneEnd();
		void Shutdown();

		namespace D2
		{
			void DrawQuad(const ObjectProperties& properties);
		}

		namespace D3
		{
			void DrawCube(const ObjectProperties& properties);
		}

	}

}
