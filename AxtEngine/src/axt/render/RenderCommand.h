#pragma once

#include "axt/Core.h"

#include "RenderAPI.h"

namespace axt {

	class AXT_API RenderCommand {
	public:
		static void Clear() { renderApi->Clear(); }
		static void SetClearColor(glm::vec4& newColor) { renderApi->SetClearColor(newColor); }
		static void DrawIndexed(const Ref<VertexArray>& vertexArray) { renderApi->DrawIndexed(vertexArray); }
	private:
		static RenderAPI* renderApi;
	};

}
