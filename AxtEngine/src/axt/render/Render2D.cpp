#include <pch.h>

#include "Render2D.h"

#include "RenderCommand.h"

//temp
#include "axt/platform/OpenGL/GLShader.h"
#include <glm/gtx/string_cast.hpp>

namespace axt {

	struct Render2DScene {
		Ref<VertexArray> mVertexArray;
		Ref<Shader> mShader;
	};

	// make this heap if the size gets pretty girthy
	static Render2DScene* sScene;

	void Render2D::Init() {
		AXT_INFO("2D INIT");
		sScene = new Render2DScene{};
		sScene->mVertexArray = VertexArray::Create();

		float lSquareVertices[4 * 3]{
			0.5f, 0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			-0.5f, -0.5f, 0.f,
			-0.5f, 0.5f, 0.f
		};
		axt::Ref<axt::VertexBuffer> lVertexBuffer{ axt::VertexBuffer::Create(lSquareVertices, sizeof(lSquareVertices)) };
		axt::BufferLayout lBufferLayout{
			{axt::ShaderDataType::Float3, "inPos"},
		};
		lVertexBuffer->SetLayout(lBufferLayout);

		sScene->mVertexArray->AddVertexBuffer(lVertexBuffer);

		uint32_t lSquareIndices[]{ 0, 1, 2, 0, 2, 3 };
		axt::Ref<axt::IndexBuffer> lIndexBuffer{ axt::IndexBuffer::Create(lSquareIndices, (sizeof(lSquareIndices) / sizeof(uint32_t))) };

		sScene->mVertexArray->AddIndexBuffer(lIndexBuffer);

		sScene->mShader = axt::Shader::Create("Shader1", "shaders/Opaque_vp.glsl", axt::ShaderType::Vertex & axt::ShaderType::Pixel);
	}

	void Render2D::Shutdown() {
		delete sScene;
	}

	void Render2D::SceneStart(const OrthoCamera& camera) {
		sScene->mShader->Bind();
		std::dynamic_pointer_cast<GLShader>(sScene->mShader)->SetValue("uViewProjection", camera.GetViewProjection());
		std::dynamic_pointer_cast<GLShader>(sScene->mShader)->SetValue("uModelTransform", glm::mat4{ 1.f });
		// spent an embarassing amount of time troubleshooting this. I was sending in a vec3 instead of mat4
		// bc SetValue has a vec3 overload, it worked without errors ;-;
	}

	void Render2D::SceneEnd() {

	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.f }, size, color);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		sScene->mShader->Bind();
		std::dynamic_pointer_cast<GLShader>(sScene->mShader)->SetValue("uColor", color);
		sScene->mVertexArray->Bind();
		RenderCommand::DrawIndexed(sScene->mVertexArray);
	}

}