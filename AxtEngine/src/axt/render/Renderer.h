#pragma once

#include "axt/Core.h"

#include "RenderCommand.h"

namespace axt {

	class AXT_API Renderer {
	public:
		static void SceneStart();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		static void SceneEnd();
		static RenderAPI::API GetApi() { return RenderAPI::GetAPI(); };
	};

}
