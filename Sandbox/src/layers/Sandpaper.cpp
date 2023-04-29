#include "Sandpaper.h"

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
// END TEMP

Sandpaper::Sandpaper(const std::string & name) : axt::Layer(),
	myCameraController{ 21 / 9 },
	myClearColor{ 0.25f, 0.25f, 0.25f, 1.f },
	mySquarePosition{ 0.f, 0.f, 0.f } 
{

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

	mySquareShader = axt::Shader::Create("Shader1", "shaders/shader2.glsl", axt::ShaderType::Vertex & axt::ShaderType::Pixel);
	myShaderLib.Add("SquareShader", mySquareShader);

	myTextureShader = axt::Shader::Create("Shader2", "shaders/shader.glsl", axt::ShaderType::Vertex & axt::ShaderType::Pixel);
	myShaderLib.Add("TexturedShader", myTextureShader);

	myTexture2D = axt::Texture2D::Create("textures/bruh.png");
	myTransparentText = axt::Texture2D::Create("textures/si.png");

	myTransparentText->Bind();
	std::dynamic_pointer_cast<axt::GLShader>(myTextureShader)->SetValue("uTexture", 0);
}

void Sandpaper::OnUpdate(float dt) {

	myCameraController.OnUpdate(dt);
	axt::RenderCommand::SetClearColor(myClearColor);
	axt::RenderCommand::Clear();

	//axt::Render3D::SceneStart(myCameraController.GetCamera());

	glm::mat4 squareTransform{ glm::translate(glm::mat4{1.f}, mySquarePosition) };

	axt::Ref<axt::Shader> currentShader{ myShaderLib.Get("SquareShader") };
	currentShader->Bind();
	std::dynamic_pointer_cast<axt::GLShader>(currentShader)->SetValue("uColor", mySquareColor);
	axt::Render3D::Submit(mySquareVertexArray, currentShader, squareTransform); //glm::translate(glm::mat4{ 1.f }, mySquarePosition));

	myShaderLib.Get("TexturedShader")->Bind();
	//axt::Render3D::Submit(mySquareVertexArray, myTextureShader);
	myTransparentText->Bind();
	//axt::Render3D::Submit(mySquareVertexArray, myTextureShader);

	//axt::Render3D::SceneEnd();
}

void Sandpaper::OnEvent(axt::Event& event) {
	myCameraController.OnEvent(event);
	axt::EventHandler handler{ event };
	handler.Fire<axt::KeyPressedEvent>(AXT_BIND_EVENT(Sandpaper::OnKeyPressedEvent));
}

void Sandpaper::OnImGuiRender() {
	ImGui::Begin("Color Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(mySquareColor));
	ImGui::End();
}

bool Sandpaper::OnKeyPressedEvent(axt::KeyPressedEvent& event) {

	return true;
}

/*
glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}
*/
