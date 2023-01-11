#include <pch.h>

#include "Renderer.h"

#include "RenderCommand.h"

namespace axt {

	void Renderer::SceneStart() {

	}

	void Renderer::SceneEnd() {

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}