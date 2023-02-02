#pragma once

#include <AxtEngine.h>

class Sand3D : public axt::Layer {
public:
	Sand3D() {};
	virtual ~Sand3D() override = default;
public:
	virtual void OnAttach() override {};
	virtual void OnDetach() override {};
	virtual void OnUpdate(float dt) override {};
	virtual void OnEvent(axt::Event& ev) override {};
	virtual void OnImGuiRender() override {};
protected:
	axt::CameraController mCamera;
};
