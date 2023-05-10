#pragma once

#include <AxtEngine.h>

#include "windows/SceneWindow.h"
#include "windows/PropertiesWindow.h"
#include "windows/ToolsWindow.h"

#include "../systems/EditorRenderSystem.h"

#include <axt/world/GameWorld.h>

#include <axt/world/CameraControlSystem.h>
#include <axt/world/RenderSystem.h>

#include <necs/include.h>

#include <imgui.h>
#include <ImGuizmo.h>

namespace axt
{

	class ELayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnEvent(Event& ev) override;
		virtual void OnImGuiRender() override;
	protected:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressed& e);
		bool OnMouseButtonReleased(MouseButtonReleased& e);
		bool NewProject();
		bool OpenProject();
		bool SaveProject();
		bool SaveProjectAs();
	public:
		ELayer();
		virtual ~ELayer() override = default;


	protected:

		struct ObjectData {
			glm::vec3 position;
			glm::vec2 size;
			glm::vec4 color;
			float rotation;
		};

		OrthoCameraController mCameraController{ 1920.f / 1080.f }; // ultrawide
		glm::vec4 mObjectColor{ 1.f, 0.47f, 0.47f, 1.f }; // salmon-ish color ig
		//glm::vec4 mClearColor{ 0.1f, 0.1f, 0.1f, 1.f };

		ObjectData obj1{ glm::vec3{0.f}, glm::vec2{1.f}, mObjectColor, 0.f };
		ObjectData obj2{ glm::vec3{0.f, 1.5f, 0.f}, glm::vec2{1.25f}, mObjectColor, 0.f };

		ImVec2 mViewportSize{};

		// temp
		Ref<Texture2D> mTexture;
		Ref<FrameBuffer> mFrameBuffer;

		Ref<GameWorld> mWorld;
		necs::Entity mCamera;

		std::string mCurrentProjectFilepath{};

		SceneOverviewWindow mSceneOverview{};
		PropertiesWindow mPropertiesWindow{};
		ToolsWindow mToolsWindow{};

		necs::Entity mSelectedEntity{ necs::nil };
		necs::Entity mHoveredEntity{ necs::nil };

		ImGuizmo::OPERATION mGizmoMode{ ImGuizmo::OPERATION::TRANSLATE };
		float mTranslationSnap{ 0.1f };
		float mRotationSnap{ 10.f };
		float mScaleSnap{ 0.25f };
		bool mSnapToggle{ false };

		bool mHoveringViewport{ false };
		bool mCursorLocked{ false };

		EditorRender mEditorRenderSystem{};

		/*Ref<necs::Scene> mScene;
		necs::Entity mCamera;
		necs::Entity mWorldRoot;

		RenderSystem mRenderSystem;
		CameraControlSystem mCameraControlSystem;

		SceneOverviewWindow mSceneOverview;
		PropertiesWindow mPropertiesWindow;
		ToolsWindow mToolsWindow;*/

	};

}