
#include <pch.h>

#include "ELayer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <axt/ecs/ComponentPack.h>
#include <axt/ecs/Scene.h>
#include <axt/ecs/Typedef.h>

static int gFps{ 0 };
static bool gDrawBulk{ true };
static float gTexRotate{ 0.f };

namespace axt
{

	ELayer::ELayer()
	{
		Unique<ecs::Scene> s{ NewUnique<ecs::Scene>() };
		ecs::EntityID i1{ s->NewEntity() };
		ecs::EntityID i2{ s->NewEntity() };
		ecs::EntityID i3{ s->NewEntity() };
		ecs::EntityID i4{ s->NewEntity() };
		s->Attach<int8_t>(i1);
		s->Attach<int8_t>(i3);
		s->Attach<uint32_t>(i2);
		s->Attach<uint32_t>(i4);
		s->Attach<int64_t>(i4);
		AXT_INFO(s->GetBitset(i1).to_string());
		AXT_INFO(s->GetBitset(i2).to_string());
		AXT_INFO(s->GetBitset(i3).to_string());
		AXT_INFO(s->GetBitset(i4).to_string());
#if 0
		AXT_INFO((ecs::gMaxEntities * sizeof(ecs::Scene::EntityInfo)));
		ecs::ComponentPack pack{ sizeof(uint32_t) };
		ecs::PackIndex i1{ pack.Add() };
		ecs::PackIndex i2{ pack.Add() };
		ecs::PackIndex i3{ pack.Add() };
		ecs::PackIndex i4{ pack.Add() };
		AXT_TRACE("{0} , {1}, {2}, {3}", i1, i2, i3, i4);
		uint32_t* mem{ nullptr };
		mem = static_cast<uint32_t*>(pack.Get(i1));
		*mem = 5;
		mem = static_cast<uint32_t*>(pack.Get(i2));
		*mem = 12;
		mem = static_cast<uint32_t*>(pack.Get(i3));
		*mem = 55;
		mem = static_cast<uint32_t*>(pack.Get(i4));
		*mem = 87;
		//AXT_INFO("{0}, {1}, {2}", *(uint32_t*)pack.Get(i1), *(uint32_t*)pack.Get(i2), *(uint32_t*)pack.Get(i3));
		//AXT_INFO("{0}, {1}, {2}", ecs::Fetch<uint32_t>(pack, i1), ecs::Fetch<uint32_t>(pack, i2), ecs::Fetch<uint32_t>(pack, i3));
		AXT_INFO("{0}, {1}, {2}, {3}", pack.FetchAs<uint32_t>(i1), pack.FetchAs<uint32_t>(i2), pack.FetchAs<uint32_t>(i3), pack.FetchAs<uint32_t>(i4));
		pack.Remove(i2);
		AXT_INFO("{0}, {1}, {2}", pack.FetchAs<uint32_t>(i1), pack.FetchAs<uint32_t>(i2), pack.FetchAs<uint32_t>(i3));
#endif
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

		RenderCommand::SetClearColor(mClearColor);
		RenderCommand::Clear();

		Render2D::SceneStart(mCameraController.GetCamera());

		Render2D::DrawQuad(Render2D::QuadProperties{ .position{0.f,0.f,-0.5f}, .size{50.f, 50.f}, .color{0.1f, 0.1f, 0.1f, 1.f}, .texName{"Check"}, .textureTiling{50.f} });

#if 1 // bulk testing
		if (gDrawBulk) {
			for (float x{ -5 }; x < 5; x += 0.5f) {
				for (float y{ -5 }; y < 5; y += 0.5f) {
					Render2D::DrawQuad(Render2D::QuadProperties{ .position{x, y, -.1f}, .size{0.45f, 0.45f}, .color{ (x + 5.f) / 10.f, 0.5f, (y + 5.f) / 10.f, 1.f} });
				}
			}
		}
#endif

		Render2D::DrawQuad(Render2D::QuadProperties{ .position{obj1.position}, .size{obj1.size}, .color{obj1.color}, .rotation{obj1.rotation} });
		Render2D::DrawQuad(Render2D::QuadProperties{ .position{obj2.position}, .size{obj2.size}, .color{obj2.color}, .texName{"Bruh"}, .rotation{obj2.rotation} });
		Render2D::DrawQuad(Render2D::QuadProperties{ .position{-2.25f, 0.f, 0.f}, .size{obj2.size}, .color{obj2.color}, .texName{"Check"}, .rotation{gTexRotate} });

		Render2D::SceneEnd();

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

		ImGui::Begin("Properties");
		ImGui::Text(">>");
		ImGui::End();

		ImGui::End(); // dockspace end
	}

}
