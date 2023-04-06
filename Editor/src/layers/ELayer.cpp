
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
		mWorld{ NewRef<World>() },
		mRenderSystem{ mWorld },
		mCameraControl{ mWorld },
		mWorldPanel{ mWorld },
		mEntityPanel{ mWorld }
	{
		mRenderSystem.SetActiveCamera(mWorld->mActiveCamera);

		static ecs::EntityID bruh{ mWorld->CreateEntity() };
		mWorld->Attach<Position>(bruh, 0.f, 0.f, 0.f);
		mWorld->Attach<Color>(bruh, 0.5f, 0.5f, 1.f, 1.f);
		mWorld->Attach<Renderable>(bruh);
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
		mCameraControl.OnEvent(ev);
	}

	void ELayer::OnUpdate(float dt)
	{
		AXT_PROFILE_FUNCTION();

		mFrameBuffer->Bind();

		mCameraController.OnUpdate(dt);

		gFps = (static_cast<int>(60.f / dt));

		mCameraControl.OnUpdate(dt, mWorld->mActiveCamera);
		mRenderSystem.OnUpdate(dt);

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

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f,0.f });
		ImGui::Begin("Viewport");
		ImVec2 fNewSize{ ImGui::GetContentRegionAvail() };
		if (mViewportSize.x != fNewSize.x || mViewportSize.y != fNewSize.y)
		{
			mViewportSize = fNewSize;
			mFrameBuffer->Resize(static_cast<uint32_t>(mViewportSize.x), static_cast<uint32_t>(mViewportSize.y));
			mCameraController.Resize(mViewportSize.x, mViewportSize.y);
			mCameraControl.Resize(mViewportSize.x, mViewportSize.y);
		}
		ImGui::Image((void*)(mFrameBuffer->GetColorTextureID()), ImVec2{ mViewportSize.x, mViewportSize.y }, { 0.f, 1.f }, { 1.f, 0.f });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Output");
		ImGui::Text(">>");
		ImGui::End();

		//ecs::EntityID selectedEntity{ mWorldPanel.OnImGuiRender(mWorld->GetRoot()) };
		//mEntityPanel.OnImGuiRender(selectedEntity);
		mEntityPanel.OnImGuiRender(mWorldPanel.OnImGuiRender());

		ImGui::End(); // dockspace end
	}

}
