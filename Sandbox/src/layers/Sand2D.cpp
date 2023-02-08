#include "Sand2D.h"

// using l__ as stack variables (even tho axt refs are heap)
// m__ as members

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <axt/platform/OpenGL/GLShader.h>

Sand2D::Sand2D() : Layer("Sand2DLayer") {

}

void Sand2D::OnAttach() {
	AXT_PROFILE_FUNCTION();

	axt::Render2D::Init();
	mTexture = axt::Texture2D::Create("textures/si.png");
}

void Sand2D::OnDetach() {
	AXT_PROFILE_FUNCTION();

	axt::Render2D::Shutdown();
}

void Sand2D::OnUpdate(float dt) {
	AXT_PROFILE_FUNCTION();

	mCameraController.OnUpdate(dt);

	axt::RenderCommand::SetClearColor(mClearColor);
	axt::RenderCommand::Clear();

	axt::Render2D::SceneStart(mCameraController.GetCamera());

	axt::Render2D::DrawQuad(axt::Render2D::QuadProperties{ .position{obj1.position}, .size{obj1.size}, .color{obj1.color}, .rotation{obj1.rotation} });
	axt::Render2D::DrawQuad(axt::Render2D::QuadProperties{ .position{obj2.position}, .size{obj2.size}, .color{obj2.color}, .texture{mTexture} });

	axt::Render2D::SceneEnd();
}

void Sand2D::OnEvent(axt::Event& ev) {
	mCameraController.OnEvent(ev);
}

void Sand2D::OnImGuiRender() {
	AXT_PROFILE_FUNCTION();

	ImGui::Begin("Control");
	ImGui::Text("Opaque Object");
	ImGui::ColorEdit4("Object Color", glm::value_ptr(obj1.color));
	ImGui::DragFloat3("Object Position", glm::value_ptr(obj1.position), 0.1f);
	ImGui::DragFloat2("Object Scale", glm::value_ptr(obj1.size));
	ImGui::DragFloat("Object Rotation", &obj1.rotation);
	ImGui::Text("Textured Object");
	ImGui::ColorEdit4("Object2 Color", glm::value_ptr(obj2.color));
	ImGui::DragFloat3("Object2 Position", glm::value_ptr(obj2.position), 0.1f);
	ImGui::DragFloat2("Object2 Scale", glm::value_ptr(obj2.size));
	ImGui::DragFloat("Object2 Rotation", &obj2.rotation);
	ImGui::End();
}
