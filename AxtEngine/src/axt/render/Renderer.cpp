#include <pch.h>

#include "Renderer.h"

#include "RenderCommand.h"

namespace axt {

	Renderer::SceneData* Renderer::scene = new Renderer::SceneData;

	void Renderer::SceneStart(const OrthoCamera& ortho) {
		scene->viewProjection = ortho.GetViewProjection();
	}

	void Renderer::SceneEnd() {

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader) {
		shader->Bind();
		shader->SetUniform("uViewProjection", scene->viewProjection);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}