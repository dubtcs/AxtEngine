
#include <pch.h>

#include "ELayer.h"

#include <axt/world/components/Transform.h>
#include <axt/world/components/Camera.h>
#include <axt/world/components/Heirarchy.h>

#include <axt/serial/Serial.h>
#include <axt/serial/Explorer.h>

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

static int gFps{ 0 };
static bool gDrawBulk{ true };
static float gTexRotate{ 0.f };

//static std::vector<axt::ecs::EntityID> gEntityIds;

namespace axt
{

	ELayer::ELayer() :
		mWorld{ NewRef<GameWorld>() }
	{

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
		//mCameraControlSystem.OnEvent(ev);
		EventHandler handler{ ev };
		handler.Fire<KeyPressedEvent>(AXT_BIND_EVENT(ELayer::OnKeyPressed));
		//handler.Fire<KeyPressedEvent>([this]() -> bool {return this->OnKeyPressed(std::forward<KeyPressedEvent>(e))});//([this]() -> bool { return this->fn(std::forward<decltype(args)>(args)...); });
	}

	void ELayer::OnUpdate(float dt)
	{
		AXT_PROFILE_FUNCTION();

		mFrameBuffer->Bind();

		mCameraController.OnUpdate(dt);

		gFps = (static_cast<int>(60.f / dt));

		//mRenderSystem.OnUpdate(dt);
		//mCameraControlSystem.OnUpdate(dt, mCamera);
		CameraControlSystem::OnUpdate(dt, mWorld);
		RenderSystem::OnUpdate(dt, mWorld);

		mFrameBuffer->Unbind();
	}

	bool ELayer::OnKeyPressed(KeyPressedEvent& e)
	{
		bool controlPressed{ input::IsKeyPressed(AXT_KEY_LEFT_CONTROL) || input::IsKeyPressed(AXT_KEY_RIGHT_CONTROL) };
		bool shiftPressed{ input::IsKeyPressed(AXT_KEY_LEFT_SHIFT) || input::IsKeyPressed(AXT_KEY_RIGHT_SHIFT) };

		switch (e.GetKeycode())
		{
			case(AXT_KEY_S) :
			{
				if (controlPressed)
				{
					if (shiftPressed)
					{
						SaveProjectAs();
						break;
					}
					else
					{
						SaveProject();
						break;
					}
				}
			}
			case(AXT_KEY_O):
			{
				if (controlPressed)
				{
					OpenProject();
					break;
				}
			}
			case(AXT_KEY_N):
			{
				if (controlPressed)
				{
					NewProject();
				}
			}
			case(AXT_KEY_1):
			{
				if (controlPressed)
				{
					mGizmoMode = ImGuizmo::OPERATION::TRANSLATE;
				}
				break;
			}
			case(AXT_KEY_2):
			{
				if (controlPressed)
				{
					mGizmoMode = ImGuizmo::OPERATION::ROTATE;
				}
				break;
			}
			case(AXT_KEY_3):
			{
				if (controlPressed)
				{
					mGizmoMode = ImGuizmo::OPERATION::SCALE;
				}
				break;
			}
		}
		return false;
	}

	bool ELayer::NewProject()
	{
		mCurrentProjectFilepath.clear();
		mWorld = NewRef<GameWorld>();
		return true;
	}

	bool ELayer::OpenProject()
	{
		std::string file{ explorer::OpenFile("Axt Project (.axtp)\0*.axtp") };
		if (!file.empty())
		{
			mCurrentProjectFilepath = file;
			mWorld = serial::Unpack(file);
			return true;
		}
		return false;
	}

	bool ELayer::SaveProject()
	{
		if (!mCurrentProjectFilepath.empty())
		{
			serial::Pack(mCurrentProjectFilepath, mWorld);
			return true;
		}
		return false;
	}

	bool ELayer::SaveProjectAs()
	{
		std::string fileName{ explorer::SaveFile("Axt Project (.axtp)\0*.axtp") };
		if (!fileName.empty())
		{
			serial::Pack(fileName + AXT_FILE_EXTENSION, mWorld);
			mCurrentProjectFilepath = fileName;
			return true;
		}
		return false;
	}

	void ELayer::OnImGuiRender()
	{
		AXT_PROFILE_FUNCTION();
		//ImGui::ShowDemoWindow();

		axt::Render2D::RenderStats fStats{ axt::Render2D::GetStats() };

		ImGuiStyle& imStyle{ ImGui::GetStyle() };
		//imStyle.WindowMenuButtonPosition = -1;

		ImGuiIO& fIO{ ImGui::GetIO() };

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

		if (fIO.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID fDockID{ ImGui::GetID("ADockspace") };
			ImGui::DockSpace(fDockID, ImVec2(0.f, 0.f), fDockspaceFlags);
		}

		if (ImGui::BeginMenuBar()) {

			//static std::string fileFormatHint{ std::format("Axt Project ({})\0*{}\0", AXT_FILE_EXTENSION, AXT_FILE_EXTENSION) };

			ImGui::Text("Axt Studio");
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New Project", "Ctrl+N"))
				{
					NewProject();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
				{
					OpenProject();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					SaveProject();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveProjectAs();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

#endif

		// end dockspace


		{

			// viewport and world related windows

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f,0.f });
			ImGui::Begin("Viewport");
			ImVec2 fNewSize{ ImGui::GetContentRegionAvail() };
			if (mViewportSize.x != fNewSize.x || mViewportSize.y != fNewSize.y)
			{
				mViewportSize = fNewSize;
				mFrameBuffer->Resize(static_cast<uint32_t>(mViewportSize.x), static_cast<uint32_t>(mViewportSize.y));
				CameraControlSystem::OnResize(fNewSize.x, fNewSize.y);

				
				AXT_TRACE("ContenRegion {0} {1}", fNewSize.x, fNewSize.y);
				ImVec2 window_size{ ImGui::GetWindowSize() };
				AXT_TRACE("WindowSize {0} {1}", window_size.x, window_size.y);
			}
			ImGui::Image((void*)(mFrameBuffer->GetColorTextureID()), ImVec2{ mViewportSize.x, mViewportSize.y }, { 0.f, 1.f }, { 1.f, 0.f });

			necs::Entity selected{ mSceneOverview.OnImGuiRender(mWorld) };
			mPropertiesWindow.OnImGuiRender(mWorld, selected);

			// Guizmo, move this to a separate window class at some point

			// necs uses nil as 0, so we can just check the value
			if (selected)
			{
				ImGuizmo::SetOrthographic(true);
				ImGuizmo::SetDrawlist();

				// gizmos
				ImVec2 viewportPosition{ ImGui::GetWindowPos() };
				ImGuizmo::SetRect(viewportPosition.x, viewportPosition.y, fNewSize.x, fNewSize.y);

				necs::Entity primaryCamera{ mWorld->GetActiveCamera() };
				if (primaryCamera && selected != primaryCamera && mWorld->HasComponent<Transform>(selected))
				{
					Camera& camera{ mWorld->GetComponent<Camera>(primaryCamera) };
					Transform& cameraTransform{ mWorld->GetComponent<Transform>(primaryCamera) };
					glm::mat4 cameraViewMatrix{ glm::inverse(cameraTransform.ToMatrix()) };

					Transform& entityTransform{ mWorld->GetComponent<Transform>(selected) };
					glm::mat4 manipulationMatrix{ entityTransform.ToMatrix() };

					float currentSnap{ 0.f };
					if (mSnapToggle || input::IsKeyPressed(AXT_KEY_LEFT_CONTROL))
					{
						switch (mGizmoMode)
						{
							case(ImGuizmo::OPERATION::TRANSLATE):
							{
								currentSnap = mTranslationSnap;
								break;
							}
							case(ImGuizmo::OPERATION::SCALE):
							{
								currentSnap = mScaleSnap;
								break;
							}
							case(ImGuizmo::OPERATION::ROTATE):
							{
								currentSnap = mRotationSnap;
								break;
							}
						}
					}

					ImGuizmo::Manipulate(glm::value_ptr(cameraViewMatrix), glm::value_ptr(camera.Projection), mGizmoMode, ImGuizmo::WORLD, glm::value_ptr(manipulationMatrix), 
						nullptr, &currentSnap);

					if (ImGuizmo::IsUsing())
					{
						ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(manipulationMatrix), glm::value_ptr(entityTransform.Position), glm::value_ptr(entityTransform.Rotation), glm::value_ptr(entityTransform.Scale));
					}
				}
			}

			ImGui::End();
			ImGui::PopStyleVar();

		}
		
		{
			ImGui::Begin("Controls");
			float width{ ImGui::GetContentRegionAvail().x };
			ImGui::PushTextWrapPos(width);
			ImGui::Text(" - Right click any empty area inside the Scene View window to open a context menu.");
			ImGui::Text(" - Right click an entity in the Scene View window to open a context menu");
			ImGui::Text(" - Ctrl + (1 - 3) : Switch gizmo transform mode between translation, rotation, and scale.");
			ImGui::End();
		}

		{
			ImGui::Begin("Whats New");
			float width{ ImGui::GetContentRegionAvail().x };
			ImGui::PushTextWrapPos(width);
			ImGui::PushFont(fIO.Fonts->Fonts[1]);
			ImGui::Text("APR 21 2023");
			ImGui::PopFont();
			ImGui::Text(" - Help and changelog windows added\n - Gizmos added for entity transform");
			ImGui::End();
		}

		ImGui::Begin("Editor Settings");

		ImGui::Text("Snap Toggled");
		ImGui::SameLine();
		ImGui::Checkbox("##STTT", &mSnapToggle);

		ImGui::Text("Translation Snap (m)");
		ImGui::SameLine();
		ImGui::InputFloat("##ET", &mTranslationSnap);

		ImGui::Text("Rotation Snap (radian)");
		ImGui::SameLine();
		ImGui::InputFloat("##ER", &mRotationSnap);

		ImGui::Text("Scale Snap (m)");
		ImGui::SameLine();
		ImGui::InputFloat("##ES", &mScaleSnap);

		ImGui::End();

		ImGui::End(); // dockspace end
	}

}
