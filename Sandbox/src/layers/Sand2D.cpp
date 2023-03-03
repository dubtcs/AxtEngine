#include "Sand2D.h"

// using l__ as stack variables (even tho axt refs are heap)
// m__ as members

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <axt/platform/OpenGL/GLShader.h>

// temp
static int gFps{ 0 };
static bool gDrawBulk{ true };
static float gTexRotate{ 0.f };

Sand2D::Sand2D() : Layer("Sand2DLayer") {

}

void Sand2D::OnAttach() {
	AXT_PROFILE_FUNCTION();

	axt::Render2D::Init();
	mTexture = axt::Texture2D::Create("textures/si.png");
	mFrameBuffer = axt::FrameBuffer::Create(axt::FrameBufferData{ .width{1920}, .height{1080} });
}

void Sand2D::OnDetach() {
	AXT_PROFILE_FUNCTION();

	axt::Render2D::Shutdown();
}

void Sand2D::OnUpdate(float dt) {
	AXT_PROFILE_FUNCTION();

	mFrameBuffer->Bind();

	mCameraController.OnUpdate(dt);

	gFps = (static_cast<int>(60.f / dt));

	axt::RenderCommand::SetClearColor(mClearColor);
	axt::RenderCommand::Clear();

	axt::Render2D::SceneStart(mCameraController.GetCamera());

	axt::Render2D::DrawQuad(axt::Render2D::QuadProperties{ .position{0.f,0.f,-0.5f}, .size{50.f, 50.f}, .color{0.1f, 0.1f, 0.1f, 1.f}, .texName{"Check"}, .textureTiling{50.f}});

#if 1 // bulk testing
	if (gDrawBulk) {
		for (float x{ -5 }; x < 5; x += 0.5f) {
			for (float y{ -5 }; y < 5; y += 0.5f) {
				axt::Render2D::DrawQuad(axt::Render2D::QuadProperties{ .position{x, y, -.1f}, .size{0.45f, 0.45f}, .color{ (x + 5.f) / 10.f, 0.5f, (y + 5.f) / 10.f, 1.f} });
			}
		}
	}
#endif

	axt::Render2D::DrawQuad(axt::Render2D::QuadProperties{ .position{obj1.position}, .size{obj1.size}, .color{obj1.color}, .rotation{obj1.rotation} });
	axt::Render2D::DrawQuad(axt::Render2D::QuadProperties{ .position{obj2.position}, .size{obj2.size}, .color{obj2.color}, .texName{"Bruh"}, .rotation{obj2.rotation} });
	axt::Render2D::DrawQuad(axt::Render2D::QuadProperties{ .position{-2.25f, 0.f, 0.f}, .size{obj2.size}, .color{obj2.color}, .texName{"Check"}, .rotation{gTexRotate} });

	axt::Render2D::SceneEnd();

	mFrameBuffer->Unbind();
}

void Sand2D::OnEvent(axt::Event& ev) {
	mCameraController.OnEvent(ev);
}

void Sand2D::OnImGuiRender() {
	AXT_PROFILE_FUNCTION();

	axt::Render2D::RenderStats fStats{ axt::Render2D::GetStats() };

	// using imgui dockspace

#if 1

	static bool sDockspace{ true };
	static bool sFullscreenPersistent{ true };
	static ImGuiDockNodeFlags fDockspaceFlags{ ImGuiDockNodeFlags_None };

	ImGuiWindowFlags fWindowFlags{ ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking };

	if (sFullscreenPersistent) {
		ImGuiViewport* fViewport{ ImGui::GetMainViewport() };
		ImGui::SetNextWindowSize(fViewport->Size);
		ImGui::SetNextWindowPos(fViewport->Pos);
		ImGui::SetNextWindowViewport(fViewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		fWindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (fDockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {
		fWindowFlags |= ImGuiWindowFlags_NoBackground;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

	ImGui::Begin("Dockspace", &sDockspace, fWindowFlags);

	ImGui::PopStyleVar();

	if (sFullscreenPersistent) {
		ImGui::PopStyleVar(2);
	}

	ImGuiIO& fIO{ ImGui::GetIO() };
	if (fIO.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID fDockID{ ImGui::GetID("ADockspace") };
		ImGui::DockSpace(fDockID, ImVec2(0.f, 0.f), fDockspaceFlags);
	}

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("AXT")) {

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

#endif

	// end dockspace

	ImGui::Begin("Control");
	ImGui::Text("Opaque Object");
	ImGui::ColorEdit4("Object Color", glm::value_ptr(obj1.color));
	ImGui::DragFloat3("Object Position", glm::value_ptr(obj1.position), 0.1f);
	ImGui::DragFloat2("Object Scale", glm::value_ptr(obj1.size), 0.1f);
	ImGui::DragFloat("Object Rotation", &obj1.rotation, 0.05f);
	ImGui::Text("Textured Object");
	ImGui::ColorEdit4("Object2 Color", glm::value_ptr(obj2.color));
	ImGui::DragFloat3("Object2 Position", glm::value_ptr(obj2.position), 0.1f);
	ImGui::DragFloat2("Object2 Scale", glm::value_ptr(obj2.size), 0.1f);
	ImGui::DragFloat("Object2 Rotation", &obj2.rotation, 0.05f);
	ImGui::Text("Object 3");
	ImGui::DragFloat("Object3 Rotation", &gTexRotate, 0.05f);
	ImGui::Text("FPS: %i", gFps);
	ImGui::Text("Draw Calls: %i", fStats.drawCalls);
	ImGui::Text("Quads: %i", fStats.quads);
	ImGui::Text("Textures: %i", fStats.textures);
	ImGui::Checkbox("Draw 400 extra quads", &gDrawBulk);
	ImGui::End();

	ImGui::Begin("Viewport");
	ImGui::Image((void*)(mFrameBuffer->GetColorTextureID()), ImVec2{ 1920.f, 1080.f }, { 0.f, 1.f }, {1.f, 0.f});
	ImGui::End();

	ImGui::End(); // dockspace end
}
