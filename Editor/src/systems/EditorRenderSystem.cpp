
#include "EditorRenderSystem.h"

#include <axt/core/Log.h>
#include <axt/render/RenderCommand.h>
#include <axt/render/Render2D.h>
#include <axt/render/Render3D.h>

#include <axt/world/GameWorld.h>
#include <axt/world/components/all.h>

static glm::vec4 gClearColor{ 0.f, 0.f, 0.f, 1.f };

namespace axt
{

	using namespace necs;

	EditorRender::EditorRender(float fieldOfView) :
		mCamera{ fieldOfView }
	{

	}

	void EditorRender::OnUpdate(float dt, Ref<GameWorld>& world, bool usable)
	{

		mCamera.OnUpdate(dt, usable);

		RenderCommand::SetClearColor(gClearColor);
		RenderCommand::Clear();

		Render2D::SceneStart(mCamera.GetViewProjectionMatrix());

		{
			SceneView<Sprite, Transform> view{ world->GetScene() };
			for (Entity id : view)
			{
				Transform& t{ world->GetComponent<Transform>(id) };
				Sprite& s{ world->GetComponent<Sprite>(id) };
				//Render2D::DrawQuad(Render2D::QuadProperties{ .position{t.Position}, .size{t.Scale}, .color{s.Color}, .EntityId{id} });
				Render2D::DrawCube(Render2D::QuadProperties{ .position{t.Position}, .size{t.Scale}, .color{s.Color}, .EntityId{id} });
			}
		}

		//Render2D::DrawQuad(Render2D::QuadProperties{ .rotation{-90.f, 0.f, 0.f}, .size{10.f, 10.f, 0.f}, .color{0.2f, 0.2f, 0.2f, 1.f}, .texName{"Check"}, .textureTiling{ 25.f } });
		Render2D::DrawCube(Render2D::QuadProperties{ .position{0.f, 5.f, 0.f}, .rotation{0.f, 0.f, 0.f}, .size{1.f, 1.f, 1.f}, .color{0.2f, 0.2f, 0.2f, 1.f}, .texName{"Bruh"}, .textureTiling{2.f} });

		Render2D::SceneEnd();

		return;

	}

	bool EditorRender::OnEvent(Event& ev)
	{
		mCamera.OnEvent(ev);
		return false;
	}

}
