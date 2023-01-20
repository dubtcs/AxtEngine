#pragma once

#include <AxtEngine.h>

class Sandbox : public axt::App {
public:
	Sandbox();
	~Sandbox() {};
};

class SandRenderLayer : public axt::Layer {
public:
	SandRenderLayer(const std::string& name = "RenderLayer");
	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(axt::Event& event) override;
	virtual void OnImGuiRender() override;
private:
	bool OnKeyPressedEvent(axt::KeyPressedEvent& event);
private:
	std::shared_ptr<axt::VertexArray> myVertexArray;
	std::shared_ptr<axt::Shader> myShader;

	std::shared_ptr<axt::Shader> mySquareShader;
	std::shared_ptr<axt::VertexArray> mySquareVertexArray;

	axt::OrthoCamera myCamera;
	glm::vec3 myCameraPosition;
	float myCameraSpeed;

	glm::vec3 mySquarePosition; // temp
	glm::vec4 mySquareColor{ 0.25f, 0.5f, 0.4f, 1.f };

	glm::vec4 myClearColor;
};
