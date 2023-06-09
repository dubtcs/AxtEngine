
#include <pch.h>

// defined to shut errors up
#define YAML_CPP_STATIC_DEFINE

#include "ELayer.h"

#include "../render/EditorCamera.h"

#include <axt/world/components/Transform.h>
#include <axt/world/components/Camera.h>
#include <axt/world/components/Heirarchy.h>

#include <axt/world/assets/Model.h>

#include <axt/serial/Serial.h>
#include <axt/serial/Explorer.h>

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

static int gFps{ 0 };
static bool gDrawBulk{ true };
static float gTexRotate{ 0.f };

namespace axt
{

	ELayer::ELayer() :
		mWorld{ NewRef<GameWorld>() }
	{

		axt::Model m{ "assets/models/multipleObjects.gltf" };

	}

	void ELayer::OnAttach()
	{
		AXT_PROFILE_FUNCTION();

		Render2D::Init();

		mTexture = Texture2D::Create("textures/si.png");

		FrameBufferData DATA{
			.Width{1920},
			.Height{1080},
			.Textures{ FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::R_INTEGER, FrameBufferTextureFormat::DEPTH24STENCIL8 }
		};

		mFrameBuffer = FrameBuffer::Create(DATA);


		// ImGuizmo Styling
		ImGuizmo::Style& style{ ImGuizmo::GetStyle() };

		style.Colors[ImGuizmo::DIRECTION_X] = ImVec4{ 1.000f, 0.419f, 0.419f, 1.f };
		style.Colors[ImGuizmo::DIRECTION_Y] = ImVec4{ 0.113f, 0.819f, 0.631f, 1.f };
		style.Colors[ImGuizmo::DIRECTION_Z] = ImVec4{ 0.372f, 0.152f, 0.803f, 1.f };

		style.Colors[ImGuizmo::PLANE_X] = ImVec4{ 1.000f, 0.419f, 0.419f, 1.f };
		style.Colors[ImGuizmo::PLANE_Y] = ImVec4{ 0.113f, 0.819f, 0.631f, 1.f };
		style.Colors[ImGuizmo::PLANE_Z] = ImVec4{ 0.372f, 0.152f, 0.803f, 1.f };

		style.Colors[ImGuizmo::SELECTION] = ImVec4{ 0.784f, 0.839f, 0.898f, 1.f };

		style.ScaleLineThickness = 5.f;
		style.TranslationLineThickness = 5.f;
		style.RotationLineThickness = 5.f;

		WindowResizeEvent spoof{ 1920, 1080 };
		mEditorRenderSystem.OnEvent(spoof);
	}

	void ELayer::OnDetach()
	{
		AXT_PROFILE_FUNCTION();

		Render2D::Shutdown();
	}

	void ELayer::OnEvent(Event& ev)
	{
		mCameraController.OnEvent(ev);
		mEditorRenderSystem.OnEvent(ev);

		EventHandler handler{ ev };
		handler.Fire<KeyPressedEvent>(AXT_BIND_EVENT(ELayer::OnKeyPressed));
		handler.Fire<MouseButtonPressed>(AXT_BIND_EVENT(ELayer::OnMouseButtonPressed));
		handler.Fire<MouseButtonReleased>(AXT_BIND_EVENT(ELayer::OnMouseButtonReleased));
	}

	void ELayer::OnUpdate(float dt)
	{
		AXT_PROFILE_FUNCTION();

		mFrameBuffer->Bind();

		mCameraController.OnUpdate(dt);

		gFps = (static_cast<int>(60.f / dt));

		mEditorRenderSystem.OnUpdate(dt, mWorld, mCursorLocked);

		mFrameBuffer->Unbind();
	}

	bool ELayer::OnKeyPressed(KeyPressedEvent& e)
	{
		bool controlPressed{ input::IsKeyPressed(Key::ControlLeft) || input::IsKeyPressed(Key::ControlRight) };
		bool shiftPressed{ input::IsKeyPressed(Key::ShiftLeft) || input::IsKeyPressed(Key::ShiftRight) };

		switch (e.GetKeycode())
		{
			case(Key::S) :
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
			case(Key::O):
			{
				if (controlPressed)
				{
					OpenProject();
					break;
				}
			}
			case(Key::N):
			{
				if (controlPressed)
				{
					NewProject();
				}
			}
			case(Key::N1):
			{
				if (controlPressed)
				{
					mGizmoMode = ImGuizmo::OPERATION::TRANSLATE;
				}
				break;
			}
			case(Key::N2):
			{
				if (controlPressed)
				{
					mGizmoMode = ImGuizmo::OPERATION::ROTATE;
				}
				break;
			}
			case(Key::N3):
			{
				if (controlPressed)
				{
					mGizmoMode = ImGuizmo::OPERATION::SCALE;
				}
				break;
			}
			case(Key::Delete):
			{
				if (mSelectedEntity)
				{
					mWorld->DestroyEntity(mSelectedEntity);
					mSelectedEntity = necs::nil;
				}
			}
		}

		return false;
	}

	bool ELayer::OnMouseButtonPressed(MouseButtonPressed& e)
	{
		if (e.GetButton() == Key::Mouse1)
		{
			if (!ImGuizmo::IsOver() && mHoveringViewport)
			{
				mSelectedEntity = mHoveredEntity;
			}
		}
		else if (e.GetButton() == Key::Mouse2)
		{
			if (mHoveringViewport && !ImGuizmo::IsUsing())
			{
				glfwSetInputMode((GLFWwindow*)(App::GetApp().GetWindow().GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				mCursorLocked = true;
			}
		}
		return false;
	}

	bool ELayer::OnMouseButtonReleased(MouseButtonReleased& e)
	{
		if (e.GetButton() == Key::Mouse2)
		{
			if (mCursorLocked)
			{
				glfwSetInputMode((GLFWwindow*)(App::GetApp().GetWindow().GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				mCursorLocked = false;
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
		imStyle.WindowMenuButtonPosition = -1;

		ImGuiIO& fIO{ ImGui::GetIO() };

		// using imgui dockspace

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

				/* TODO:
				/  Fix gizmos being offset when the viewport title bar is shown. 
				/  Usually a few pixels below where it should be. Y{0} isn't at the top of the viewport, it's in the title bar when visible :\
				*/
				WindowResizeEvent spoof{ static_cast<int>(fNewSize.x), static_cast<int>(fNewSize.y) };
				mEditorRenderSystem.OnEvent(spoof);
			}
			ImGui::Image((void*)(mFrameBuffer->GetColorTextureID()), ImVec2{ mViewportSize.x, mViewportSize.y }, { 0.f, 1.f }, { 1.f, 0.f });

			// MOUSE SELECTION
			// // Can maybe switch to bounding boxes and raycasting.
			// // I just wanted to get something up and running without over complicating things too early

		
			// Mouse Selection
			ImVec2 windowPosition{ ImGui::GetWindowPos() };
			ImVec2 windowSize{ ImGui::GetWindowSize() };
			ImVec2 mousePosition{ ImGui::GetMousePos() };

			// also viewport position
			ImVec2 mouseDelta{ mousePosition.x - windowPosition.x, mousePosition.y - windowPosition.y };
			mHoveringViewport = mouseDelta.x > 0 && mouseDelta.x <= windowSize.x && mouseDelta.y > 0 && mouseDelta.y <= windowSize.y;
			if (mHoveringViewport)
			{
				mFrameBuffer->Bind();
				// flippedY is needed because the texture is flipped
				int32_t flippedY{ static_cast<int32_t>(windowSize.y) - static_cast<int32_t>(mouseDelta.y) };
				uint32_t entity{ mFrameBuffer->GetPixelData(1, static_cast<int32_t>(mouseDelta.x), flippedY) };
				mHoveredEntity = entity;
				// we don't even need to clear the buffer bc necs::nil is 0 :)
				mFrameBuffer->Unbind();
			}
			else
			{
				mHoveredEntity = necs::nil;
			}

			mSelectedEntity = mSceneOverview.OnImGuiRender(mWorld, mSelectedEntity);
			mPropertiesWindow.OnImGuiRender(mWorld, mSelectedEntity);
			mToolsWindow.OnImGuiRender(mWorld);

			// Guizmo, move this to a separate window class at some point
			// necs uses nil as 0, so we can just check the value
			if (mSelectedEntity)
			{
				const EditorCamera& camera{ mEditorRenderSystem.GetCamera() };

				ImGuizmo::SetOrthographic(!camera.IsPerspective());
				ImGuizmo::SetDrawlist();

				// gizmos
				ImVec2 viewportPosition{ ImGui::GetWindowPos() };
				ImGuizmo::SetRect(viewportPosition.x, viewportPosition.y, fNewSize.x, fNewSize.y);

				glm::vec3 currentSnap{ 0.f };
				if (mSnapToggle || input::IsKeyPressed(Key::ControlLeft))
				{
					switch (mGizmoMode)
					{
						case(ImGuizmo::OPERATION::TRANSLATE):
						{
							currentSnap = { mTranslationSnap, mTranslationSnap, mTranslationSnap };
							break;
						}
						case(ImGuizmo::OPERATION::SCALE):
						{
							currentSnap = { mScaleSnap, mScaleSnap, mScaleSnap };
							break;
						}
						case(ImGuizmo::OPERATION::ROTATE):
						{
							currentSnap = { mRotationSnap, mRotationSnap, mRotationSnap };
							break;
						}
						
					}
				}

				if (mWorld->HasComponent<Transform>(mSelectedEntity))
				{
					Transform& entityTransform{ mWorld->GetComponent<Transform>(mSelectedEntity) };
					glm::mat4 manipulationMatrix{ entityTransform.ToMatrix() };

					ImGuizmo::Manipulate(glm::value_ptr(camera.GetViewMatrix()), glm::value_ptr(camera.GetProjectionMatrix()), mGizmoMode, ImGuizmo::WORLD, glm::value_ptr(manipulationMatrix),
						nullptr, glm::value_ptr(currentSnap));

					if (ImGuizmo::IsUsing())
					{
						ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(manipulationMatrix), glm::value_ptr(entityTransform.Position), glm::value_ptr(entityTransform.Rotation), glm::value_ptr(entityTransform.Scale));
					}
				}

			}
			else
			{
				//AXT_WARN("6. THE DOLLAR");
			}

			ImGui::End();
			ImGui::PopStyleVar();

		}
		
		{
			ImGui::Begin("Controls");
			float width{ ImGui::GetContentRegionAvail().x };
			ImGui::PushTextWrapPos(width);

			ImGui::PushFont(fIO.Fonts->Fonts[1]);
			ImGui::Text("Scene View");
			ImGui::PopFont();
			ImGui::Text(" - Right click any empty area inside the Scene View window to open a context menu.");
			ImGui::Text(" - Right click an entity in the Scene View window to open a context menu");
			ImGui::Text(" - Ctrl + (1 - 3) : Switch gizmo transform mode between translation, rotation, and scale.");
			ImGui::Text(" - Delete : Destroy the currently selected entity.");

			ImGui::Separator();
			ImGui::PushFont(fIO.Fonts->Fonts[1]);
			ImGui::Text("Viewport");
			ImGui::PopFont();
			ImGui::Text(" - Hold right click to enable camera movement.");
			ImGui::Text(" \t- WASD for movement.");
			ImGui::Text(" \t- Q/E for vertical camera movement.");
			ImGui::Text(" - Hold left control to enable snapping.");

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

		ImGui::Separator();
		ImGui::Text("Camera");
		
		ImGuiInputTextFlags flags{ ImGuiInputTextFlags_EnterReturnsTrue };

		/*
		TODO:
		Stop accessing the member variables directly.
		*/

		ImGui::Text("Field of View");
		ImGui::SameLine();
		float sratchfloat{ mEditorRenderSystem.mCamera.mFOV };
		if (ImGui::InputFloat("##CFOV", &sratchfloat, 0.f, 0.f, nullptr, flags))
		{
			mEditorRenderSystem.mCamera.mFOV = sratchfloat;
			mEditorRenderSystem.mCamera.BuildProjectionMatrix();
			mEditorRenderSystem.mCamera.BuildMatrices();
		}

		ImGui::Text("Sensitivity");
		ImGui::SameLine();
		sratchfloat = mEditorRenderSystem.mCamera.mSensitivity;
		if (ImGui::InputFloat("##CSENS", &sratchfloat, 0.f, 0.f, nullptr, flags))
		{
			mEditorRenderSystem.mCamera.mSensitivity = sratchfloat;
		}

		ImGui::Text("Movement Speed");
		ImGui::SameLine();
		sratchfloat = mEditorRenderSystem.mCamera.mMovementSpeed;
		if (ImGui::InputFloat("##CMVM", &sratchfloat, 0.f, 0.f, nullptr, flags))
		{
			mEditorRenderSystem.mCamera.mMovementSpeed = sratchfloat;
		}

		ImGui::End();

		ImGui::End(); // dockspace end
	}

}
