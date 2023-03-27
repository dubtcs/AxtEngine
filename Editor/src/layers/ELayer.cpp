
#include <pch.h>

#include "ELayer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <axt/ecs/SceneView.h>

#include <entt/entt.hpp>

static int gFps{ 0 };
static bool gDrawBulk{ true };
static float gTexRotate{ 0.f };

struct Transform
{
	glm::vec3 Position;
};

namespace axt
{

	ELayer::ELayer() :
		mScene{ NewRef<ecs::Scene>() },
		OBJ{ mScene }
	{
#if 0
		constexpr uint32_t EAMOUNT{ 1'000'000 };

		/*
		Create 10'000 entities
		attach transforms
		create a view
		*/

		struct Transform
		{
			glm::vec3 Position;
		};

		ecs::Entity e{ mScene };
		e.Attach<Transform>();
#endif
#if 0
		ecs::EntityID ids[10]{};

		AXT_INFO("Assignment:");
		for (uint32_t i{ 0 }; i < 10; i++)
		{
			ids[i] = mScene->CreateEntity();
			Transform& t{ mScene->Attach<Transform>(ids[i]) };
			t.Position.x = static_cast<float>(i);
			AXT_TRACE("{0} : {0}", i);
		}

		AXT_INFO("Deletion");
		for (uint32_t i{ 0 }; i < 5; i++)
		{
			mScene->Detach<Transform>(ids[i * 2]);
		}

		ecs::SceneView<Transform> view{ mScene };
		for (ecs::EntityID e : view)
		{
			Transform& t{ mScene->GetComponent<Transform>(e) };
			AXT_TRACE("Transform: {0} : {1}", e, t.Position.x);
		}
#endif

#if 0
		{
			AXT_PROFILE_SCOPE("nECS");
			using namespace ecs;

			std::vector<EntityID> ids;

			{
				AXT_PROFILE_SCOPE("Create entities");
				for (uint32_t i{ 0 }; i < EAMOUNT; i++)
				{
					ids.push_back(mScene->CreateEntity());
				}
			}

			{
				AXT_PROFILE_SCOPE("Attach transforms");
				for (EntityID& i : ids)
				{
					mScene->Attach<Transform>(i);
				}
			}

			{
				AXT_PROFILE_SCOPE("Create scene view");
				SceneView<Transform> view{ mScene };
				{
					AXT_PROFILE_SCOPE("Assign transform values");
					for (EntityID id : view)
					{
						Transform& t{ mScene->GetComponent<Transform>(id) };
						t.Position.x = 126.4f;
						//AXT_INFO(mScene->GetComponent<Transform>(ids.at(id))->Position.x);
					}
				}

			}
		}

		{
			AXT_PROFILE_SCOPE("ENTT");
			using namespace entt;

			registry r{};

			std::vector<entity> ids;
			{
				AXT_PROFILE_SCOPE("Create entities");
				for (uint32_t i{ 0 }; i < EAMOUNT; i++)
				{
					ids.push_back(r.create());
				}
			}

			{
				AXT_PROFILE_SCOPE("Attach transforms");
				for (entity& i : ids)
				{
					r.emplace<Transform>(i);
				}
			}

			{
				AXT_PROFILE_SCOPE("Create scene view");
				auto view{ r.view<Transform>() };
				{
					AXT_PROFILE_SCOPE("Assign transform values");
					for (entity id : view)
					{
						auto& t = view.get<Transform>(id);
						t.Position.x = 126.4f;
						//AXT_INFO(mScene->GetComponent<Transform>(ids.at(id))->Position.x);
					}
				}
			}

		}
#endif

#if 0 // test Scene Destroy propagating to each pack
		using namespace ecs;

		Unique<Scene> s{ NewUnique<Scene>() };
		EntityID i1{ s->CreateEntity() };
		s->Attach<uint32_t>(i1);
		s->Attach<uint8_t>(i1);
		EntityID i2{ s->CreateEntity() };
		s->Attach<uint8_t>(i2);

		s->DestroyEntity(i2);
#endif
#if 0 // Testing new replacements
		Unique<IDManager> m{ NewUnique<IDManager>() };
		Unique<ComponentPack> p{ NewUnique<ComponentPack>(sizeof(uint32_t)) };

		EntityID id[]{ 0,1,2,3 };

		for (EntityID i : id)
		{
			p->Add(i);
			uint32_t* v{ static_cast<uint32_t*>(p->Get(i)) };
			*v = i*55;
			AXT_TRACE(p->FetchAs<uint32_t>(i));
		}

		p->Remove(id[2]);

		for (EntityID i : id)
			AXT_TRACE(p->FetchAs<uint32_t>(i));

#endif

#if 0
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
#endif
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

		OBJ.Attach<Transform>();
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

		//Transform& T{ mScene->GetComponent<Transform>(o1ID) };
		Transform& T{ OBJ.GetComponent<Transform>() };
		Render2D::DrawQuad(Render2D::QuadProperties{ .position{T.Position}, .size{obj1.size}, .color{obj1.color}, .rotation{obj1.rotation} });
		//Render2D::DrawQuad(Render2D::QuadProperties{ .position{obj2.position}, .size{obj2.size}, .color{obj2.color}, .texName{"Bruh"}, .rotation{obj2.rotation} });
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

		//Transform& T{ mScene->GetComponent<Transform>(o1ID) };
		Transform& T{ OBJ.GetComponent<Transform>() };
		ImGui::DragFloat3("Object Position", glm::value_ptr(T.Position), 0.1f);

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
