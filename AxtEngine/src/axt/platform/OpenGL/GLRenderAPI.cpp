#include <pch.h>

#include "GLRenderAPI.h"

#include <glad/glad.h>

namespace axt {

	void GLRenderAPI::Init() 
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	void GLRenderAPI::Clear() const 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRenderAPI::SetClearColor(const glm::vec4& newColor) const 
	{
		glClearColor(newColor.x, newColor.y, newColor.z, newColor.a);
	}

	void GLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count) const 
	{
		//count = (count == 0) ? vertexArray->GetIndexBuffer()->GetCount() : count; // why was I forcing this to draw everything??
		//AXT_CORE_TRACE("{0} -> Drawing: {1} verts", __FUNCTION__, count);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void GLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) const 
	{
		glViewport(x, y, w, h);
	}

}