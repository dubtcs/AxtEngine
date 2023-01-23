#pragma once

#include "axt/Core.h"

#include <glm/glm.hpp>
#include <memory>

#include "VertexArray.h"

namespace axt {

	class AXT_API RenderAPI {
	public:
		enum class API {
			None = 0, OpenGL
		};
	public:
		virtual void Clear() const = 0;
		virtual void SetClearColor(const glm::vec4& newColor) const = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) const = 0;
		static API GetAPI() { return api; }
	private:
		static API api;
	};

}
