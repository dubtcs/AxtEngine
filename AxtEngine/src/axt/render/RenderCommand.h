#pragma once

#include "axt/Core.h"

#include "RenderAPI.h"

namespace axt {

	class AXT_API RenderCommand {
	public:
		static void Init() { renderApi->Init(); }
		static void Clear() { renderApi->Clear(); }
		static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) { renderApi->SetViewport(x, y, w, h); }
		static void SetClearColor(glm::vec4& newColor) { renderApi->SetClearColor(newColor); }
		static void DrawIndexed(const Ref<VertexArray>& vertexArray) { renderApi->DrawIndexed(vertexArray); }
	private:
		static RenderAPI* renderApi;
	};

}
