#pragma once

#include "axt/Core.h"
#include "axt/render/RenderAPI.h"

namespace axt {

	class AXT_API GLRenderAPI : public RenderAPI {
		virtual void Init() override;
		virtual void Clear() const override;
		virtual void SetClearColor(const glm::vec4& newColor) const override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) const override;
	};

}
