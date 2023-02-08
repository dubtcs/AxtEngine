#include <pch.h>

#include "Render2D.h"

#include "RenderCommand.h"
#include <glm/ext/matrix_transform.hpp>

namespace axt {

	struct QuadVert {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoordinate;
	};

	struct Render2DScene {
		Ref<VertexArray> mVertexArray;
		Ref<VertexBuffer> mVertexBuffer;
		Ref<Shader> mShader;
		Ref<Texture2D> mTexture;

		uint32_t mIndexCount{ 0 };
		const uint32_t mMaxQuads{ 10000 };
		const uint32_t mMaxVertices{ mMaxQuads * 4 };
		const uint32_t mMaxIndices{ mMaxQuads * 6 };

		QuadVert* mQuadStart{ nullptr }; // starting point of quad array
		QuadVert* mCurrentQuadVertex{ nullptr }; // current index in scene
	};

	static Render2DScene* sScene;

	/*
	Batch render:
	Create VertexArray
	Create VertexBuffer with max storage
	Bufferlayout based on QuadVert
	Index buffer
	*/

	void Render2D::Init() {
		AXT_PROFILE_FUNCTION();

		sScene = new Render2DScene{};
		sScene->mVertexArray = VertexArray::Create();
		sScene->mVertexBuffer = VertexBuffer::Create(sScene->mMaxVertices * sizeof(QuadVert));

		// QuadVert structure
		axt::BufferLayout lBufferLayout{
			{axt::ShaderDataType::Float3, "inPos"},
			{axt::ShaderDataType::Float4, "inColor"},
			{axt::ShaderDataType::Float2, "inTexPos"},
		};
		sScene->mVertexBuffer->SetLayout(lBufferLayout);
		sScene->mVertexArray->AddVertexBuffer(sScene->mVertexBuffer);

		sScene->mQuadStart = new QuadVert[sScene->mMaxQuads];

		Unique<uint32_t[]> fIndexData{ NewUnique<uint32_t[]>(sScene->mMaxIndices) };
		uint32_t fIndexOffset{ 0 };
		for (uint32_t i{ 0 }; i < sScene->mMaxIndices; i += 6) {
			fIndexData[i] = fIndexOffset;
			fIndexData[i + 1] = fIndexOffset + 1;
			fIndexData[i + 2] = fIndexOffset + 2;

			fIndexData[i + 3] = fIndexOffset + 2;
			fIndexData[i + 4] = fIndexOffset + 3;
			fIndexData[i + 5] = fIndexOffset + 0;

			fIndexOffset += 4;
		}
		Ref<IndexBuffer> fIndexBuffer{ IndexBuffer::Create(fIndexData.get(), sScene->mMaxIndices)};
		sScene->mVertexArray->AddIndexBuffer(fIndexBuffer);

		sScene->mShader = axt::Shader::Create("Shader1", "shaders/bruh.glsl", axt::ShaderType::Vertex | axt::ShaderType::Pixel);
	}

	void Render2D::Shutdown() {

		delete sScene;
	}

	void Render2D::SceneStart(const OrthoCamera& camera) {
		AXT_PROFILE_FUNCTION();
		sScene->mCurrentQuadVertex = sScene->mQuadStart;
		sScene->mIndexCount = 0;

		sScene->mShader->Bind();
		sScene->mShader->SetValue("uViewProjection", camera.GetViewProjection());
	}

	void Render2D::SceneEnd() {
		AXT_PROFILE_FUNCTION();

		uint32_t fDataSize{ static_cast<uint32_t>((char*)sScene->mCurrentQuadVertex - (char*)sScene->mQuadStart) };
		sScene->mVertexBuffer->SubmitData(sScene->mQuadStart, fDataSize);

		RenderCommand::DrawIndexed(sScene->mVertexArray, sScene->mIndexCount);
	}

	void Render2D::Flush() {

	}

	void Render2D::DrawQuad(const QuadProperties&& fQuad) {
		AXT_PROFILE_FUNCTION();

		if (sScene->mIndexCount + 4 > sScene->mMaxQuads) {
			AXT_CORE_ASSERT(false, "Render2D::DrawQuad QuadAmount limit overflow!");
			return;
		}

		// bottom left
		sScene->mCurrentQuadVertex->position = fQuad.position;
		sScene->mCurrentQuadVertex->color = fQuad.color;
		sScene->mCurrentQuadVertex->textureCoordinate = { 0.f, 0.f };
		sScene->mCurrentQuadVertex++;

		// bottom right
		sScene->mCurrentQuadVertex->position = { fQuad.position.x + fQuad.size.x, fQuad.position.y, 0.f };
		sScene->mCurrentQuadVertex->color = fQuad.color;
		sScene->mCurrentQuadVertex->textureCoordinate = { 1.f, 0.f };
		sScene->mCurrentQuadVertex++;

		//top right
		sScene->mCurrentQuadVertex->position = { fQuad.position.x + fQuad.size.x, fQuad.position.y + fQuad.size.y, 0.f };
		sScene->mCurrentQuadVertex->color = fQuad.color;
		sScene->mCurrentQuadVertex->textureCoordinate = { 1.f, 1.f };
		sScene->mCurrentQuadVertex++;

		// top left
		sScene->mCurrentQuadVertex->position = { fQuad.position.x, fQuad.position.y + fQuad.size.y, 0.f };
		sScene->mCurrentQuadVertex->color = fQuad.color;
		sScene->mCurrentQuadVertex->textureCoordinate = { 0.f, 1.f };
		sScene->mCurrentQuadVertex++;

		sScene->mIndexCount += 6;
	}

	//void Render2D::DrawQuad(const QuadProperties&& fQuad) {
	void Foing(const Render2D::QuadProperties&& fQuad ) {
		AXT_PROFILE_FUNCTION();
		glm::mat4 fIdentityMatrix{ 1.f };
		glm::mat4 fTransform{ glm::translate(fIdentityMatrix, fQuad.position) * glm::scale(fIdentityMatrix, glm::vec3{fQuad.size.x, fQuad.size.y, 0.f}) }; 
		if (fQuad.rotation != 0.f) {
			fTransform *= glm::rotate(fIdentityMatrix, glm::radians(fQuad.rotation), glm::vec3{ 0.f, 0.f, 1.f });
		}
		sScene->mShader->SetValue("uModelTransform", fTransform);
		sScene->mShader->SetValue("uColor", fQuad.color);
		if (fQuad.texture) {
			fQuad.texture->Bind(); // texture
		}
		else {
			sScene->mTexture->Bind(); // white texture
		}
		sScene->mVertexArray->Bind();
		RenderCommand::DrawIndexed(sScene->mVertexArray);
	}

}