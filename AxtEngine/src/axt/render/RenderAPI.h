#pragma once

#include "axt/Core.h"

#include <glm/glm.hpp>
#include <memory>

#include "VertexArray.h"

namespace axt {

	struct RenderStats {
		int fps{ 0 };
	};

	class AXT_API RenderAPI {
	public:
		enum class API {
			None = 0, OpenGL
		};
	public:
		virtual void Init() = 0;
		virtual void Clear() const = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) const = 0;
		virtual void SetClearColor(const glm::vec4& newColor) const = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) const = 0;
		static API GetAPI() { return api; }
	private:
		static API api;
	};

}
