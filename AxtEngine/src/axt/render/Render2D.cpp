#include <pch.h>

#include "Render2D.h"

#include "RenderCommand.h"
#include <glm/ext/matrix_transform.hpp>

namespace axt {

	struct Render2DScene {
		Ref<VertexArray> mVertexArray;
		Ref<Shader> mShader;
		Ref<Texture2D> mTexture;
	};

	// make this heap if the size gets pretty girthy
	static Render2DScene* sScene;

	void Render2D::Init() {
		AXT_PROFILE_FUNCTION();

		sScene = new Render2DScene{};
		sScene->mVertexArray = VertexArray::Create();

		float lSquareVertices[4 * 5]{
			0.5f, 0.5f, 0.f, 0.f, 0.f,
			0.5f, -0.5f, 0.f, 1.f, 0.f,
			-0.5f, -0.5f, 0.f, 1.f, 1.f,
			-0.5f, 0.5f, 0.f, 0.f, 1.f
		};
		axt::Ref<axt::VertexBuffer> lVertexBuffer{ axt::VertexBuffer::Create(lSquareVertices, sizeof(lSquareVertices)) };
		axt::BufferLayout lBufferLayout{
			{axt::ShaderDataType::Float3, "inPos"},
			{axt::ShaderDataType::Float2, "inTexPos"},
		};
		lVertexBuffer->SetLayout(lBufferLayout);

		sScene->mVertexArray->AddVertexBuffer(lVertexBuffer);

		uint32_t lSquareIndices[]{ 0, 1, 2, 0, 2, 3 };
		axt::Ref<axt::IndexBuffer> lIndexBuffer{ axt::IndexBuffer::Create(lSquareIndices, (sizeof(lSquareIndices) / sizeof(uint32_t))) };

		sScene->mVertexArray->AddIndexBuffer(lIndexBuffer);

		sScene->mShader = axt::Shader::Create("Shader1", "shaders/aio_vp.glsl", axt::ShaderType::Vertex | axt::ShaderType::Pixel);

		sScene->mTexture = axt::Texture2D::Create(1, 1);
		uint32_t lTexData{ 0xffffffff };
		sScene->mTexture->SetData(&lTexData, sizeof(lTexData));
	}

	void Render2D::Shutdown() {
		delete sScene;
	}

	void Render2D::SceneStart(const OrthoCamera& camera) {
		AXT_PROFILE_FUNCTION();

		sScene->mShader->Bind();
		sScene->mShader->SetValue("uViewProjection", camera.GetViewProjection());
	}

	void Render2D::SceneEnd() {

	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const float& rotation) {
		DrawQuad({ position.x, position.y, 0.f }, size, color);
	}

	void Render2D::DrawQuad(const QuadProperties&& fQuad) {
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





	// DEPRECATED
	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const float& rotation) {
		AXT_PROFILE_FUNCTION();

		glm::mat4 lIdentityMatrix{ 1.f };
		glm::mat4 lTransform{ glm::translate(lIdentityMatrix, position) * glm::scale(lIdentityMatrix, glm::vec3{size.x, size.y, 0.f}) * glm::rotate(lIdentityMatrix, glm::radians(rotation), glm::vec3{0.f, 0.f, 1.f}) };
		sScene->mShader->SetValue("uModelTransform", lTransform);
		sScene->mShader->SetValue("uColor", color);
		sScene->mTexture->Bind(); // using white texture
		sScene->mVertexArray->Bind();
		RenderCommand::DrawIndexed(sScene->mVertexArray);
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D> texture, const float& rotation) {
		DrawQuad({ position.x, position.y, 0.f }, size, texture, rotation);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture, const float& rotation) {
		AXT_PROFILE_FUNCTION();

		glm::mat4 lIdentityMatrix{ 1.f };
		glm::mat4 lTransform{ glm::translate(lIdentityMatrix, position) * glm::scale(lIdentityMatrix, glm::vec3{size.x, size.y, 0.f}) * glm::rotate(lIdentityMatrix, glm::radians(rotation), glm::vec3{0.f, 0.f, 1.f}) };
		sScene->mShader->SetValue("uModelTransform", lTransform);
		sScene->mShader->SetValue("uColor", glm::vec4{ 1.f });
		texture->Bind(); // using custom texture
		sScene->mVertexArray->Bind();
		RenderCommand::DrawIndexed(sScene->mVertexArray);
	}


}