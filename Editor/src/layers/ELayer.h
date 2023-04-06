#pragma once

#include <AxtEngine.h>
#include <imgui.h>

#include <axt/world/World.h>
#include <axt/world/RenderSystem.h>
#include <axt/world/CameraControlSystem.h>

#include "windows/SceneOverview.h"
#include "windows/Properties.h"

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

		//Ref<ecs::Scene> mScene;
		Ref<World> mWorld;

		RenderSystem mRenderSystem;
		CameraControlSystem mCameraControl;

		// Windows/Panels
		WorldOverviewPanel mWorldPanel;
		PropertiesPanel mEntityPanel;

		//Ref<World> mWorld;
	};

}