#include <pch.h>

#include "Render3D.h"

#include "RenderCommand.h"

#include <necs/include.h>
#include <glm/glm.hpp>

//temp
#include "axt/platform/OpenGL/GLShader.h"
#include <glm/gtx/string_cast.hpp>

// this is awful lmao

namespace axt {

	constexpr uint32_t MAX_3D_TEXTURE_UNITS{ 32 };

	struct CubeVert
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoordinate;
		float textureId = 0;
		uint32_t entityId{ necs::nil };
	};

	static constexpr uint32_t MAX_CUBES{ 1'000 };

	struct SceneData
	{
		glm::mat4 viewProjection;
		Ref<VertexArray> VAO;
		Ref<VertexBuffer> VBO;
		Ref<Shader> Shade;

		uint32_t mTexturesUsed{ 1 };
		uint32_t mVertexCount{ 0 };
		uint32_t mIndexCount{ 0 };

		const uint32_t mMaxVertices{ MAX_CUBES * 8 };
		const uint32_t mMaxIndices{ MAX_CUBES * 12 };

		CubeVert* mCubeStart{ nullptr }; // starting point of cube array
		CubeVert* mCurrentVertex{ nullptr }; // current index in scene

	};

	static SceneData* scene{ new SceneData{} };

	static const glm::vec4 gCubeVerts[] {
		glm::vec4{  1.f,   1.f,   1.f,  1.f },
		glm::vec4{ -1.f,   1.f,   1.f,  1.f },
		glm::vec4{ -1.f,  -1.f,   1.f,  1.f },
		glm::vec4{  1.f,  -1.f,   1.f,  1.f },

		glm::vec4{  1.f,   1.f,  -1.f,  1.f },
		glm::vec4{ -1.f,   1.f,  -1.f,  1.f },
		glm::vec4{ -1.f,  -1.f,  -1.f,  1.f },
		glm::vec4{  1.f,  -1.f,  -1.f,  1.f }
	};

	void Render3D::Init()
	{
		scene->VAO = VertexArray::Create();
		scene->VBO = VertexBuffer::Create(scene->mMaxVertices * sizeof(CubeVert));

		BufferLayout bufferLayout
		{
			{axt::ShaderDataType::Float3, "inPos"},
			{axt::ShaderDataType::Float4, "inColor"},
			{axt::ShaderDataType::Float2, "inTexPos"},
			{axt::ShaderDataType::Float, "inTexId"},

			{axt::ShaderDataType::UInt, "inEntityId"}
		};

		scene->VBO->SetLayout(bufferLayout);
		scene->VAO->AddVertexBuffer(scene->VBO);

		scene->mCubeStart = new CubeVert[MAX_CUBES * 8];

		Unique<uint32_t[]> indexData{ NewUnique<uint32_t[]>(scene->mMaxIndices) };
		uint32_t indexOffset{ 0 };

		for (uint32_t i{ 0 }; i < scene->mMaxIndices; i += 12)
		{
			indexData[i     ] = indexOffset;
			indexData[i +  1] = indexOffset + 1;
			indexData[i +  2] = indexOffset + 2;

			indexData[i +  3] = indexOffset + 2;
			indexData[i +  4] = indexOffset + 3;
			indexData[i +  5] = indexOffset + 0;

			indexData[i +  6] = indexOffset + 3;
			indexData[i +  7] = indexOffset + 4;
			indexData[i +  8] = indexOffset + 5;

			indexData[i +  9] = indexOffset + 5;
			indexData[i + 10] = indexOffset + 6;
			indexData[i + 11] = indexOffset + 3;

			indexOffset += 8;
		}

		Ref<IndexBuffer> indexBuffer{ IndexBuffer::Create(indexData.get(), scene->mMaxIndices) };
		scene->VAO->AddIndexBuffer(indexBuffer);

		scene->Shade = axt::Shader::Create("Shader1", "shaders/aio2_vp.glsl", axt::ShaderType::Vertex | axt::ShaderType::Pixel);

	}

	void Render3D::Shutdown()
	{
		delete scene->mCubeStart;
		delete scene;
	}

	void Render3D::SceneStart(const glm::mat4& viewProjection)
	{
		scene->viewProjection = viewProjection;

		AXT_PROFILE_FUNCTION();
		scene->mCurrentVertex = scene->mCubeStart;
		scene->mIndexCount = 0;
		scene->mVertexCount = 0;

		scene->Shade->Bind();
		scene->Shade->SetValue("uViewProjection", viewProjection);
	}

	void Render3D::SceneEnd()
	{
		uint32_t fDataSize{ static_cast<uint32_t>((char*)scene->mCurrentVertex - (char*)scene->mCubeStart) };
		scene->VBO->SubmitData(scene->mCubeStart, fDataSize);
		Stage();
	}

	void Render3D::Stage()
	{
		RenderCommand::DrawIndexed(scene->VAO, scene->mIndexCount);
	}

	void Render3D::DrawCube(const CubeProperties& props)
	{
		if (scene->mIndexCount + 12 > scene->mMaxIndices)
		{
			//if( gData->mVertexCount + 4 > gData->mMaxVertices) {
				//AXT_CORE_ASSERT(false, "Render2D::DrawQuad QuadAmount limit overflow!");
			//StageForOverflow();
		}

		// model space transform
		const glm::mat4 fIdMat{ 1.f };
		const glm::mat4 fModelTransform{
			glm::translate(fIdMat, props.Position) * // can trim the rotation from this if it's not rotated
			//glm::rotate(fIdMat, props.Rotation, glm::vec3{0.f,0.f,1.f}) *
			glm::scale(fIdMat, glm::vec3{props.Scale.x, props.Scale.y, 1.f})
		};

		for (const glm::vec4& fCurrentPosition : gCubeVerts)
		{
			scene->mCurrentVertex->position = fModelTransform * fCurrentPosition;;
			scene->mCurrentVertex->color = props.Color;
			//scene->mCurrentVertex->textureCoordinate = { (fCurrentPosition.x + 0.5f) * props.TextureTiling, (fCurrentPosition.y + 0.5f) * props.TextureTiling };
			//scene->mCurrentVertex->textureId = fTexId;

			// EDITOR SHADER ONLY
			// REMOVE WHEN BETTER MOUSE SELECTION IS ADDED
			//scene->mCurrentVertex->entityId = props.EntityId;

			scene->mCurrentVertex++;

			scene->mVertexCount++;
		}

		scene->mIndexCount += 6;
		//gStats.quads++;
	}





	void Render3D::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& modelTransform) {
		shader->Bind();

		// temp
		std::dynamic_pointer_cast<GLShader>(shader)->SetValue("uViewProjection", scene->viewProjection);
		std::dynamic_pointer_cast<GLShader>(shader)->SetValue("uModelTransform", modelTransform);
		AXT_TRACE(glm::to_string(modelTransform));
		// endtemp

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}