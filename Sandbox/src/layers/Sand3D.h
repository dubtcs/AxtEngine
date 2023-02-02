#pragma once

#include <AxtEngine.h>

class Sand3D : public axt::Layer {
public:
	Sand3D(const std::string& name = "Sand3D-Layer");
	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(axt::Event& event) override;
	virtual void OnImGuiRender() override;
private:
	bool OnKeyPressedEvent(axt::KeyPressedEvent& event);
private:
	axt::OrthoCameraController myCameraController;
	axt::ShaderLib myShaderLib;

	axt::Ref<axt::VertexArray> myVertexArray;
	axt::Ref<axt::Shader> myShader;

	axt::Ref<axt::Shader> mySquareShader;
	axt::Ref<axt::VertexArray> mySquareVertexArray;

	axt::Ref<axt::Shader> myTextureShader;

	axt::Ref<axt::Texture2D> myTexture2D, myTransparentText;

	glm::vec3 mySquarePosition; // temp
	glm::vec4 mySquareColor{ 0.25f, 0.5f, 0.4f, 1.f };

	glm::vec4 myClearColor;
};
