
// CLIENT

#include "sndbx.h"

#include <axt/core/OpenShader.h>
#include <axt/render/Camera.h>

// TEMP

#include <glm/glm.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

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
	myCameraSpeed{ 0.1f } {

	myVertexArray.reset(axt::VertexArray::Create());

	float verts[7 * 3]{
		0.0f, .5f, 0.0f,   0.8f, 0.5f, 0.1f, 1.f,
		-0.5f, -0.5f, 0.0f, 1.f, 0.0f, 0.35f, 1.f,
		0.5f, -0.5f, 0.0f, 0.8f, 0.25f, 0.6f, 1.f
	};

	std::shared_ptr<axt::VertexBuffer> vBuffer;
	vBuffer.reset(axt::VertexBuffer::Create(verts, sizeof(verts)));
	{
		axt::BufferLayout vLayout{
			{axt::ShaderDataType::Float3, "inPos"},
			{axt::ShaderDataType::Float4, "inColor"},
		};

		vBuffer->SetLayout(vLayout);
	}
	myVertexArray->AddVertexBuffer(vBuffer);

	uint32_t ind[3]{ 0, 1, 2 }; // had this set to 1,2,3 ffs
	std::shared_ptr<axt::IndexBuffer> iBuffer;
	iBuffer.reset(axt::IndexBuffer::Create(ind, 3));
	myVertexArray->AddIndexBuffer(iBuffer);

	//myCamera.SetPosition({ 0.25f, 0.5f, 0.f });
	myCamera.SetRotation(.5f);

	float squareVertices[3 * 4]{
		0.5f, 0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		-0.5f, 0.5f, 0.f
	};

	mySquareVertexArray.reset(axt::VertexArray::Create());
	std::shared_ptr<axt::VertexBuffer> squareVB;
	squareVB.reset(axt::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

	{
		axt::BufferLayout squareLayout{
			{axt::ShaderDataType::Float3, "inPos"}
		};
		squareVB->SetLayout(squareLayout);
	}
	mySquareVertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[]{ 0,1,2,0,2,3 };
	std::shared_ptr<axt::IndexBuffer> squareIB;
	squareIB.reset(axt::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	mySquareVertexArray->AddIndexBuffer(squareIB);

	// Shaders
	std::string vertexPath{ "shaders/v.vert" };
	std::string pixelPath{ "shaders/f.frag" };

	myShader.reset(axt::Shader::Create(axt::OpenShader(vertexPath), axt::OpenShader(pixelPath)));

	vertexPath = "shaders/square.vert";

	mySquareShader.reset(axt::Shader::Create(axt::OpenShader(vertexPath), axt::OpenShader(pixelPath)));
}

void SandRenderLayer::OnUpdate() {

	if (axt::AxtInput::IsKeyPressed(AXT_KEY_A)) {
		myCameraPosition.x -= myCameraSpeed;
	} else if (axt::AxtInput::IsKeyPressed(AXT_KEY_D)) {
		myCameraPosition.x += myCameraSpeed;
	}
	if (axt::AxtInput::IsKeyPressed(AXT_KEY_W)) {
		myCameraPosition.y += myCameraSpeed;
	} else if (axt::AxtInput::IsKeyPressed(AXT_KEY_S)) {
		myCameraPosition.y -= myCameraSpeed;
	}

	axt::RenderCommand::SetClearColor(myClearColor);
	axt::RenderCommand::Clear();

	std::string viewProjectionUniformName{ "uViewProjection" };

	const glm::mat4& viewProjection{ myCamera.GetViewProjection() };
	myCamera.SetPosition(myCameraPosition);

	axt::Renderer::SceneStart(myCamera);

	axt::Renderer::Submit(mySquareVertexArray, mySquareShader);
	axt::Renderer::Submit(myVertexArray, myShader);

	axt::Renderer::SceneEnd();
}

void SandRenderLayer::OnEvent(axt::Event& event) {
	axt::EventHandler handler{ event };
	handler.Fire<axt::KeyPressedEvent>(AXT_BIND_EVENT(SandRenderLayer::OnKeyPressedEvent));
}

bool SandRenderLayer::OnKeyPressedEvent(axt::KeyPressedEvent& event) {

	return true;
}
