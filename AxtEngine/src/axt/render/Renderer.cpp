#include <pch.h>

#include "Renderer.h"

#include "RenderCommand.h"




//temp
#include "axt/platform/OpenGL/GLShader.h"

namespace axt {

	Render3D::SceneData* Render3D::scene = new Render3D::SceneData;

	void Render3D::Init() {
		RenderCommand::Init();
	}

	void Render3D::SceneStart(const OrthoCamera& ortho) {
		scene->viewProjection = ortho.GetViewProjection();
	}

	void Render3D::SceneEnd() {

	}

	void Render3D::WindowResized(const uint32_t x, const uint32_t y) {
		RenderCommand::SetViewport(0, 0, x, y);
	}

	void Render3D::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& modelTransform) {
		shader->Bind();

		// temp
		std::dynamic_pointer_cast<GLShader>(shader)->SetValue("uViewProjection", scene->viewProjection);
		std::dynamic_pointer_cast<GLShader>(shader)->SetValue("uModelTransform", modelTransform);
		// endtemp

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}