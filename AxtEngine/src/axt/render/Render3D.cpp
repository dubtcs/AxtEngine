#include <pch.h>

#include "Render3D.h"

#include "RenderCommand.h"

//temp
#include "axt/platform/OpenGL/GLShader.h"
#include <glm/gtx/string_cast.hpp>

namespace axt {

	//Render3D::SceneData* Render3D::scene = new Render3D::SceneData;
	Unique<Render3D::SceneData> Render3D::scene = NewUnique<Render3D::SceneData>();

	void Render3D::SceneStart(const OrthoCamera& ortho) {
		scene->viewProjection = ortho.GetViewProjection();
	}

	void Render3D::SceneEnd() {

	}

	void Render3D::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& modelTransform) {
		shader->Bind();

		// temp
		std::dynamic_pointer_cast<GLShader>(shader)->SetValue("uViewProjection", scene->viewProjection);
		std::dynamic_pointer_cast<GLShader>(shader)->SetValue("uModelTransform", modelTransform);
		AXT_TRACE(glm::to_string(modelTransform));
		// endtemp

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}