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
	void OnUpdate() override;
private:
	std::shared_ptr<axt::VertexArray> myVertexArray;
	std::shared_ptr<axt::Shader> myShader;

	std::shared_ptr<axt::Shader> mySquareShader;
	std::shared_ptr<axt::VertexArray> mySquareVertexArray;

	axt::OrthoCamera myCamera;

	glm::vec4 myClearColor;
};
