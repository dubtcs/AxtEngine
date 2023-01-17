
// client

#include "sndbx.h"

#include "axt/core/OpenShader.h"
#include "axt/render/Camera.h"

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

// LAYER

// APP
Sandbox::Sandbox() {
	using namespace axt;

	vArray.reset(VertexArray::Create());

	float verts[7 * 3]{
		0.0f, .5f, 0.0f,   0.8f, 0.5f, 0.1f, 1.f,
		-0.5f, -0.5f, 0.0f, 1.f, 0.0f, 0.35f, 1.f,
		0.5f, -0.5f, 0.0f, 0.8f, 0.25f, 0.6f, 1.f
	};

	std::shared_ptr<VertexBuffer> vBuffer;
	vBuffer.reset(VertexBuffer::Create(verts, sizeof(verts)));
	{
		BufferLayout vLayout{
			{ShaderDataType::Float3, "inPos"},
			{ShaderDataType::Float4, "inColor"},
		};

		vBuffer->SetLayout(vLayout);
	}
	vArray->AddVertexBuffer(vBuffer);

	uint32_t ind[3]{ 0, 1, 2 }; // had this set to 1,2,3 ffs
	std::shared_ptr<IndexBuffer> iBuffer;
	iBuffer.reset(IndexBuffer::Create(ind, 3));
	vArray->AddIndexBuffer(iBuffer);

	//myCamera.SetPosition({ 0.25f, 0.5f, 0.f });
	myCamera.SetRotation(.5f);

	float squareVertices[3 * 4]{
		0.5f, 0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		-0.5f, 0.5f, 0.f
	};

	squareArray.reset(VertexArray::Create());
	std::shared_ptr<VertexBuffer> squareVB;
	squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

	{
		BufferLayout squareLayout{
			{ShaderDataType::Float3, "inPos"}
		};
		squareVB->SetLayout(squareLayout);
	}
	squareArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[]{ 0,1,2,0,2,3 };
	std::shared_ptr<IndexBuffer> squareIB;
	squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	squareArray->AddIndexBuffer(squareIB);

	// Shaders
	std::string vertexPath{ "shaders/v.vert" };
	std::string pixelPath{ "shaders/f.frag" };

	shader.reset(Shader::Create(OpenShader(vertexPath), OpenShader(pixelPath)));

	vertexPath = "shaders/square.vert";

	squareShader.reset(Shader::Create(OpenShader(vertexPath), OpenShader(pixelPath)));
}
