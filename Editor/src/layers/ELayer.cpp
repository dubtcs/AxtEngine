
#include <pch.h>

#include "ELayer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <entt/entt.hpp>

#include <axt/world/Components.h>

static int gFps{ 0 };
static bool gDrawBulk{ true };
static float gTexRotate{ 0.f };

static std::vector<axt::ecs::EntityID> gEntityIds;

namespace axt
{

	ELayer::ELayer() :
		mScene{ NewRef<ecs::Scene>() },
		mCamera{ mScene->CreateEntity() },
		mRenderSystem{},
		mCameraControl{ mScene }
	{
		mScene->Attach<Camera>(mCamera, { -1.78f, 1.78f, -1.f, 1.f });
		mScene->Attach<Position>(mCamera);
		mRenderSystem.SetActiveCamera(mCamera);

		static ecs::EntityID bruh{ mScene->CreateEntity() };
		mScene->Attach<Position>(bruh, { 0.f, -0.5f, 0.f });
		mScene->Attach<Color>(bruh, {0.25f, 0.5f, 1.f, 1.f});
		mScene->Attach<Renderable>(bruh);
	}

	void ELayer::OnAttach()
	{
		AXT_PROFILE_FUNCTION();
		Render2D::Init();
		mTexture = Texture2D::Create("textures/si.png");
		mFrameBuffer = FrameBuffer::Create(FrameBufferData{ .width{1920}, .height{1080} });
	}

	void ELayer::OnDetach()
	{
		AXT_PROFILE_FUNCTION();

		Render2D::Shutdown();
	}

	void ELayer::OnEvent(Event& ev)
	{
		mCameraController.OnEvent(ev);
	}

	void ELayer::OnUpdate(float dt)
	{
		AXT_PROFILE_FUNCTION();

		mFrameBuffer->Bind();

		mCameraController.OnUpdate(dt);

		gFps = (static_cast<int>(60.f / dt));

		mCameraControl.OnUpdate(dt, mCamera);
		mRenderSystem.OnUpdate(dt, mScene);

		mFrameBuffer->Unbind();
	}

	void ELayer::OnImGuiRender()
	{
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

		//Transform& T{ mScene->GetComponent<Transform>(o1ID) };
		//Transform& T{ OBJ.GetComponent<Transform>() };
		//ImGui::DragFloat3("Object Position", glm::value_ptr(T.Position), 0.1f);

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

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f,0.f });
		ImGui::Begin("Viewport");
		ImVec2 fNewSize{ ImGui::GetContentRegionAvail() };
		if (mViewportSize.x != fNewSize.x || mViewportSize.y != fNewSize.y)
		{
			mViewportSize = fNewSize;
			mFrameBuffer->Resize(static_cast<uint32_t>(mViewportSize.x), static_cast<uint32_t>(mViewportSize.y));
			mCameraController.Resize(mViewportSize.x, mViewportSize.y);
		}
		ImGui::Image((void*)(mFrameBuffer->GetColorTextureID()), ImVec2{ mViewportSize.x, mViewportSize.y }, { 0.f, 1.f }, { 1.f, 0.f });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Output");
		ImGui::Text(">>");
		ImGui::End();

		ImGui::Begin("Instances");
		if (ImGui::Button("Add Square"))
		{
			ecs::EntityID id{ mScene->CreateEntity() };
			gEntityIds.push_back(id);
			mScene->Attach<Position>(id);
			mScene->Attach<Color>(id, { 0.25f, 0.25f, 0.25f });
		}
		for (ecs::EntityID& i : gEntityIds)
		{
			Position& t{ mScene->GetComponent<Position>(i) };
			Color& c{ mScene->GetComponent<Color>(i) };
			ImGui::PushID(static_cast<int>(i));
			ImGui::Text("Object: %i", i);
			ImGui::DragFloat3("Rotation", glm::value_ptr(t.Value), 0.05f);
			ImGui::ColorEdit4("Color", glm::value_ptr(c.Value));
			if (ImGui::Button("Delete"))
			{
				mScene->DestroyEntity(i);
				gEntityIds.erase(std::find(gEntityIds.begin(), gEntityIds.end(), i));
			}
			ImGui::PopID();
		}
		ImGui::End();

		ImGui::Begin("Properties");
		ImGui::Text(">>");
		ImGui::End();

		ImGui::End(); // dockspace end
	}

}
