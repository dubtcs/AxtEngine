#include <pch.h>

#include "VertexArray.h"

#include "Renderer.h"
#include "axt/platform/OpenGL/GLVertexArray.h"

namespace axt{

	VertexArray* VertexArray::Create() {
		switch (Renderer::GetApi()) {
		case(RenderAPI::OpenGL): return new GLVertexArray{};
		}
		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

}
