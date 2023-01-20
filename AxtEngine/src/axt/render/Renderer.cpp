#include <pch.h>

#include "Renderer.h"

#include "RenderCommand.h"




//temp
#include "axt/platform/OpenGL/GLShader.h"

namespace axt {

	Renderer::SceneData* Renderer::scene = new Renderer::SceneData;

	void Renderer::SceneStart(const OrthoCamera& ortho) {
		scene->viewProjection = ortho.GetViewProjection();
	}

	void Renderer::SceneEnd() {

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& modelTransform) {
		shader->Bind();
		// temp
		std::dynamic_pointer_cast<GLShader>(shader)->SetValue("uViewProjection", scene->viewProjection);
		std::dynamic_pointer_cast<GLShader>(shader)->SetValue("uModelTransform", modelTransform);
		// endtemp

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}