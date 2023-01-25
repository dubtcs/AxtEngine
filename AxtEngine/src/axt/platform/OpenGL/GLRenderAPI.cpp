#include <pch.h>

#include "GLRenderAPI.h"

#include <glad/glad.h>

namespace axt {

	void GLRenderAPI::Init() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void GLRenderAPI::Clear() const {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GLRenderAPI::SetClearColor(const glm::vec4& newColor) const {
		glClearColor(newColor.x, newColor.y, newColor.z, newColor.a);
	}

	void GLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) const {
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}