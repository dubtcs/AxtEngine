#pragma once

#include <AxtEngine.h>

class Sand2D : public axt::Layer {
public:
	Sand2D();
	virtual ~Sand2D() override = default;
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(axt::Event& ev) override;
	virtual void OnImGuiRender() override;
protected:
	axt::OrthoCameraController mCameraController{ 3440.f/1440.f }; // ultrawide
	glm::vec4 mObjectColor{ 1.f, 0.47f, 0.47f, 1.f }; // salmon-ish color ig
	glm::vec4 mClearColor{ 0.25f, 0.25f, 0.25f, 1.f };

	// temp
	axt::Ref<axt::VertexArray> mVertexArray;
	axt::Ref<axt::Shader> mShader;
};
