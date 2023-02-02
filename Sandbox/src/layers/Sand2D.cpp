#include "Sand2D.h"

// using l__ as stack variables (even tho axt refs are heap)
// m__ as members

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <axt/platform/OpenGL/GLShader.h>

Sand2D::Sand2D() : Layer("Sand2DLayer") {

}

void Sand2D::OnAttach() {
	axt::Render2D::Init();
}

void Sand2D::OnDetach() {
	axt::Render2D::Shutdown();
}

void Sand2D::OnUpdate(float dt) {
	mCameraController.OnUpdate(dt);

	axt::RenderCommand::SetClearColor(mClearColor);
	axt::RenderCommand::Clear();

	axt::Render2D::SceneStart(mCameraController.GetCamera());
	axt::Render2D::DrawQuad(glm::vec3{ 0.f }, glm::vec3{ 1.f }, mObjectColor);
	axt::Render2D::SceneEnd();
}

void Sand2D::OnEvent(axt::Event& ev) {
	mCameraController.OnEvent(ev);
}

void Sand2D::OnImGuiRender() {
	ImGui::Begin("Control");
	ImGui::ColorEdit4("Object Color", glm::value_ptr(mObjectColor));
	ImGui::End();
}
