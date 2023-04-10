#include <pch.h>

#include "RenderSystem.h"

#include "components/Camera.h"
#include "components/Transform.h"

#include <axt/render/Render2D.h>
#include <axt/render/RenderCommand.h>

#include "components/all.h"

static glm::vec4 gClearColor{ 0.1f, 0.1f, 0.1f, 1.f };

namespace axt
{

	using namespace necs;

	RenderSystem::RenderSystem(Ref<Scene>& scene) : System{ scene }
	{

	}

	bool RenderSystem::OnUpdate(float dt)
	{
		Camera& camera{ mScene->GetComponent<Camera>(mCamera) };
		Transform& t{ mScene->GetComponent<Transform>(mCamera) };

		glm::mat4 ones{ 1.f };
		glm::mat4 transformMatrix{ glm::translate(ones, t.Position) * glm::rotate(ones, glm::radians(t.Rotation.x), glm::vec3{0,0,1.f}) };
		glm::mat4 viewProjection{ glm::inverse(transformMatrix) };
		viewProjection = camera.Projection * viewProjection;

		RenderCommand::SetClearColor(gClearColor);
		RenderCommand::Clear();

		Render2D::SceneStart(camera, viewProjection);

		{
			SceneView<Sprite, Transform> view{ mScene };
			for (Entity id : view)
			{
				Transform& t{ mScene->GetComponent<Transform>(id) };
				Sprite& s{ mScene->GetComponent<Sprite>(id) };
				Render2D::DrawQuad(Render2D::QuadProperties{ .position{t.Position}, .size{s.Size}, .color{s.Color} });
			}
		}

		Render2D::SceneEnd();

		return true;
	}

	void RenderSystem::SetActiveCamera(const Entity& id)
	{
		mCamera = id;
	}

}