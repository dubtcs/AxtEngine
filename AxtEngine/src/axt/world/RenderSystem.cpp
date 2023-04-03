#include <pch.h>

#include "RenderSystem.h"

#include "axt/ecs/nECS.h"
#include "Components.h"

#include "axt/render/RenderCommand.h"
#include "axt/render/Render2D.h"

static glm::vec4 mClearColor{ 0.1f, 0.1f, 0.1f, 1.f };

namespace axt
{

	using namespace ecs;

	bool RenderSystem::OnUpdate(float dt, const Ref<Scene>& scene)
	{

		Camera& camera{ scene->GetComponent<Camera>(mCameraID) };
		Position& cameraPosition{ scene->GetComponent<Position>(mCameraID) };

		glm::mat4 ones{ 1.f };
		glm::mat4 transformMatrix{ glm::translate(ones, cameraPosition.Value) * glm::rotate(ones, glm::radians(0.f), glm::vec3{0,0,1.f}) };
		glm::mat4 viewProjection{ glm::inverse(transformMatrix) };
		viewProjection = camera.Projection * viewProjection;

		RenderCommand::SetClearColor(mClearColor);
		RenderCommand::Clear();

		Render2D::SceneStart(camera, viewProjection);

		// 2D
		{
			SceneView<Renderable, Position, Color> view{ scene };
			for (EntityID id : view)
			{
				Position& position{ scene->GetComponent<Position>(id) };
				Color& color{ scene->GetComponent<Color>(id) };
				Render2D::DrawQuad(Render2D::QuadProperties{ .position{position.Value}, .size{1.f, 1.f}, .color{color.Value} });
			}
		}

		Render2D::SceneEnd();

		return true;
	}

	void RenderSystem::SetActiveCamera(const EntityID& id)
	{
		mCameraID = id;
	}

}