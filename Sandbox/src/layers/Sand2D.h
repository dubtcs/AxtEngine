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
	struct ObjectData {
		glm::vec3 position;
		glm::vec2 size;
		glm::vec4 color;
		float rotation;
	};

	axt::OrthoCameraController mCameraController{ 1920.f / 1080.f }; // ultrawide
	glm::vec4 mObjectColor{ 1.f, 0.47f, 0.47f, 1.f }; // salmon-ish color ig
	glm::vec4 mClearColor{ 0.25f, 0.25f, 0.25f, 1.f };

	ObjectData obj1{ glm::vec3{0.f}, glm::vec2{1.f}, mObjectColor, 0.f };
	ObjectData obj2{ glm::vec3{1.f, 1.f, 0.f}, glm::vec2{1.25f}, mObjectColor, 0.f };

	// temp
	axt::Ref<axt::Texture2D> mTexture;
};
