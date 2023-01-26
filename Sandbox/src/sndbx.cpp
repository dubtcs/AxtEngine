
// CLIENT

#include "sndbx.h"

#include <axt/core/OpenShader.h>
#include <axt/render/Camera.h>

// TEMP
#include <imgui.h>
#include <axt/platform/OpenGL/GLShader.h>
#include <axt/platform/OpenGL/GLTexture.h>

#include <glm/glm.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/type_ptr.hpp>

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

// END TEMP

// ENTRY POINT
axt::App* axt::CreateApp() {
	return new Sandbox{};
}

// APP
Sandbox::Sandbox() {
	PushLayer(new SandRenderLayer{});
}

// LAYERS
SandRenderLayer::SandRenderLayer(const std::string& name) : axt::Layer(), 
	myCamera{ -1.f, 1.f, -1.f, 1.f }, 
	myClearColor{ 0.25f, 0.25f, 0.25f, 1.f },
	myCameraPosition{ 0.f,0.f,0.f },
	myCameraSpeed{ 1.f },
	mySquarePosition{ 0.f, 0.f, 0.f } {

	myVertexArray = axt::VertexArray::Create();

	float verts[7 * 3]{
		0.0f, .5f, 0.0f,   0.8f, 0.5f, 0.1f, 1.f,
		-0.5f, -0.5f, 0.0f, 1.f, 0.0f, 0.35f, 1.f,
		0.5f, -0.5f, 0.0f, 0.8f, 0.25f, 0.6f, 1.f
	};

	axt::Ref<axt::VertexBuffer> vBuffer;
	vBuffer = axt::VertexBuffer::Create(verts, sizeof(verts));
	{
		axt::BufferLayout vLayout{
			{axt::ShaderDataType::Float3, "inPos"},
			{axt::ShaderDataType::Float4, "inColor"},
		};

		vBuffer->SetLayout(vLayout);
	}
	myVertexArray->AddVertexBuffer(vBuffer);

	uint32_t ind[3]{ 0, 1, 2 }; // had this set to 1,2,3 ffs
	axt::Ref<axt::IndexBuffer> iBuffer;
	iBuffer = axt::IndexBuffer::Create(ind, 3);
	myVertexArray->AddIndexBuffer(iBuffer);

	float squareVertices[5 * 4]{
		0.5f, 0.5f, 0.f, 1.f, 1.f,
		0.5f, -0.5f, 0.f, 1.f, 0.f,
		-0.5f, -0.5f, 0.f, 0.f, 0.f,
		-0.5f, 0.5f, 0.f, 0.f, 1.f
	};

	mySquareVertexArray = axt::VertexArray::Create();
	axt::Ref<axt::VertexBuffer> squareVB;
	squareVB = axt::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	{
		axt::BufferLayout squareLayout{
			{axt::ShaderDataType::Float3, "inPos"},
			{axt::ShaderDataType::Float2, "inTexPos"}
		};
		squareVB->SetLayout(squareLayout);
	}
	mySquareVertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[]{ 0,1,2,0,2,3 };
	axt::Ref<axt::IndexBuffer> squareIB;
	squareIB = axt::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	mySquareVertexArray->AddIndexBuffer(squareIB);

	// Shaders
	std::string vertexPath{ "shaders/v.vert" };
	std::string pixelPath{ "shaders/flatColor.frag" };

	myShader = axt::Shader::Create(axt::OpenShader(vertexPath), axt::OpenShader(pixelPath));

	vertexPath = "shaders/square.vert";

	mySquareShader = axt::Shader::Create(axt::OpenShader(vertexPath), axt::OpenShader(pixelPath));

	pixelPath = "shaders/tPos.frag";
	vertexPath = "shaders/tPos.vert";

	myTextureShader = axt::Shader::Create(axt::OpenShader(vertexPath), axt::OpenShader(pixelPath));

	myTexture2D = axt::Texture2D::Create("textures/bruh.png");
	myTransparentText = axt::Texture2D::Create("textures/si.png");
	
	myTransparentText->Bind();
	std::dynamic_pointer_cast<axt::GLShader>(myTextureShader)->SetValue("uTexture", 0);
}

void SandRenderLayer::OnUpdate(float dt) {

	if (axt::AxtInput::IsKeyPressed(AXT_KEY_A)) {
		myCameraPosition.x -= myCameraSpeed * dt;
	} else if (axt::AxtInput::IsKeyPressed(AXT_KEY_D)) {
		myCameraPosition.x += myCameraSpeed * dt;
	}
	if (axt::AxtInput::IsKeyPressed(AXT_KEY_W)) {
		myCameraPosition.y += myCameraSpeed * dt;
	} else if (axt::AxtInput::IsKeyPressed(AXT_KEY_S)) {
		myCameraPosition.y -= myCameraSpeed * dt;
	}

	// square movement
	if (axt::AxtInput::IsKeyPressed(AXT_KEY_UP)) {
		mySquarePosition.y += myCameraSpeed * dt;
	}
	else if (axt::AxtInput::IsKeyPressed(AXT_KEY_DOWN)) {
		mySquarePosition.y -= myCameraSpeed * dt;
	}
	if (axt::AxtInput::IsKeyPressed(AXT_KEY_RIGHT)) {
		mySquarePosition.x += myCameraSpeed * dt;
	}
	else if (axt::AxtInput::IsKeyPressed(AXT_KEY_LEFT)) {
		mySquarePosition.x -= myCameraSpeed * dt;
	}

	axt::RenderCommand::SetClearColor(myClearColor);
	axt::RenderCommand::Clear();

	std::string viewProjectionUniformName{ "uViewProjection" };

	const glm::mat4& viewProjection{ myCamera.GetViewProjection() };
	myCamera.SetPosition(myCameraPosition);

	axt::Renderer::SceneStart(myCamera);

	glm::mat4 squareTransform{ glm::translate(glm::mat4{1.f}, mySquarePosition) };

	mySquareShader->Bind();
	std::dynamic_pointer_cast<axt::GLShader>(mySquareShader)->SetValue("uColor", mySquareColor);
	axt::Renderer::Submit(mySquareVertexArray, mySquareShader, squareTransform);//glm::translate(glm::mat4{ 1.f }, mySquarePosition));

	myShader->Bind();
	std::dynamic_pointer_cast<axt::GLShader>(myShader)->SetValue("uColor", { 0.1f, 0.6f, 0.8f, 1.f });
	axt::Renderer::Submit(myVertexArray, myShader);

	myTextureShader->Bind();
	axt::Renderer::Submit(mySquareVertexArray, myTextureShader);
	myTransparentText->Bind();
	axt::Renderer::Submit(mySquareVertexArray, myTextureShader);

	axt::Renderer::SceneEnd();
}

void SandRenderLayer::OnEvent(axt::Event& event) {
	axt::EventHandler handler{ event };
	handler.Fire<axt::KeyPressedEvent>(AXT_BIND_EVENT(SandRenderLayer::OnKeyPressedEvent));
}

void SandRenderLayer::OnImGuiRender() {
	ImGui::Begin("Color Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(mySquareColor));
	ImGui::End();
}

bool SandRenderLayer::OnKeyPressedEvent(axt::KeyPressedEvent& event) {

	return true;
}
