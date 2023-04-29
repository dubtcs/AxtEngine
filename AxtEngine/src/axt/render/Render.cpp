#include <pch.h>

#include "Render.h"

#include "VertexArray.h"
#include "Buffers.h"
#include "Shader.h"
#include "Texture.h"

namespace axt
{

	namespace render
	{

		static constexpr uint32_t MAX_TEXTURE_UNITS{ 32 };

		static constexpr uint32_t MAX_OBJECTS_2D{ 100 };
		static constexpr uint32_t MAX_VERTICES_2D{ MAX_OBJECTS_2D * 4 };
		static constexpr uint32_t MAX_INDICES_2D{ MAX_OBJECTS_2D * 6 };

		static constexpr uint32_t MAX_OBJECTS_3D{ 50 };
		static constexpr uint32_t MAX_VERTICES_3D{ MAX_OBJECTS_3D * 8 };
		static constexpr uint32_t MAX_INDICES_3D{ MAX_OBJECTS_2D * 36 }; // Using GL_TRIANGLES

		struct RenderSceneData
		{
			Ref<VertexArray> VArray;
			Ref<VertexBuffer> VBuffer2D;
			Ref<VertexBuffer> VBuffer3D;

			Ref<Shader> ShaderOpaque;
			Ref<Texture2D> TextureWhite;

			uint32_t TexturesUsed{ 1 };
			uint32_t VertexCount{ 0 };
			uint32_t IndexCount{ 0 };

			VertexData* QuadStart{ nullptr };
			VertexData* QuadCurrent{ nullptr };

			VertexData* CubeStart{ nullptr };
			VertexData* CubeCurrent{ nullptr };

			TextureLib TextureLibrary;
			std::array<Ref<Texture2D>, MAX_TEXTURE_UNITS> TextureArray;
		};

		struct VertexData
		{
			glm::vec3 Position;
			glm::vec2 TextureCoordinate;
			glm::vec4 Color;
			float TextureID{ 0 };
			uint32_t EntityID{ 0 };
		};

		RenderSceneData* gScene{ nullptr };

		void Init()
		{
			gScene = new RenderSceneData{};

			gScene->VArray = VertexArray::Create();

			gScene->VBuffer2D = VertexBuffer::Create(MAX_VERTICES_2D * sizeof(ObjectProperties));
			gScene->VBuffer3D = VertexBuffer::Create(MAX_VERTICES_3D * sizeof(ObjectProperties));

			BufferLayout bufferLayout
			{
				{axt::ShaderDataType::Float3, "inPos"},
				{axt::ShaderDataType::Float4, "inColor"},
				{axt::ShaderDataType::Float2, "inTexPos"},
				{axt::ShaderDataType::Float, "inTexId"},

				{axt::ShaderDataType::UInt, "inEntityId"}
			};

			gScene->VBuffer2D->SetLayout(bufferLayout);
			gScene->VBuffer3D->SetLayout(bufferLayout);

			gScene->QuadStart = new VertexData[MAX_VERTICES_2D];
			gScene->CubeStart = new VertexData[MAX_VERTICES_3D];

			uint32_t indexOffset{ 0 };

			uint32_t* indexData2D{ new uint32_t[MAX_INDICES_2D] };
			for (uint32_t i{ 0 }; i < MAX_INDICES_2D; i += 6)
			{
				indexData2D[i] = indexOffset;
				indexData2D[i + 1] = indexOffset + 1;
				indexData2D[i + 2] = indexOffset + 2;

				indexData2D[i + 3] = indexOffset + 2;
				indexData2D[i + 4] = indexOffset + 3;
				indexData2D[i + 5] = indexOffset + 0;

				indexOffset += 4;
			}

			indexOffset = 0;

			// TODO: Expand render to support multiple buffers

		}

		void Shutdown()
		{
			delete gScene;
		}

	}

}