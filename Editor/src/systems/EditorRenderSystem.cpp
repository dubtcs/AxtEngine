
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
				Render2D::DrawQuad(Render2D::QuadProperties{ .position{t.Position}, .size{t.Scale}, .color{s.Color}, .EntityId{id} });
			}
		}

		Render2D::SceneEnd();

		return;

	}

	bool EditorRender::OnEvent(Event& ev)
	{
		mCamera.OnEvent(ev);
		return false;
	}

}
