#include "Sand2D.h"

// using l__ as stack variables (even tho axt refs are heap)
// m__ as members

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <axt/platform/OpenGL/GLShader.h>

Sand2D::Sand2D() : Layer("Sand2D-Layer") {

}

void Sand2D::OnAttach() {
	mVertexArray = axt::VertexArray::Create();

	// raw data
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

	mVertexArray->AddVertexBuffer(lVertexBuffer);

	uint32_t lSquareIndices[]{ 0,1,2,0,2,3 };
	axt::Ref<axt::IndexBuffer> lIndexBuffer{ axt::IndexBuffer::Create( lSquareIndices, (sizeof(lSquareIndices) / sizeof(uint32_t)) ) };

	mVertexArray->AddIndexBuffer(lIndexBuffer);

	mShader = axt::Shader::Create("Shader1", "shaders/Opaque_vp.glsl", axt::ShaderType::Vertex & axt::ShaderType::Pixel);
}

void Sand2D::OnDetach() {

}

void Sand2D::OnUpdate(float dt) {
	mCameraController.OnUpdate(dt);
	axt::RenderCommand::SetClearColor(mClearColor);
	axt::RenderCommand::Clear();

	axt::Render3D::SceneStart(mCameraController.GetCamera());

	mShader->Bind();
	std::dynamic_pointer_cast<axt::GLShader>(mShader)->SetValue("uColor", mObjectColor);
	axt::Render3D::Submit(mVertexArray, mShader);

	axt::Render3D::SceneEnd();
}

void Sand2D::OnEvent(axt::Event& ev) {
	mCameraController.OnEvent(ev);
}

void Sand2D::OnImGuiRender() {
	ImGui::Begin("Control");
	ImGui::ColorEdit4("Object Color", glm::value_ptr(mObjectColor));
	ImGui::End();
}
