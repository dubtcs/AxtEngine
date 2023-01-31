#include <pch.h>

#include "VertexArray.h"

#include "Renderer.h"
#include "axt/platform/OpenGL/GLVertexArray.h"

namespace axt{

	Ref<VertexArray> VertexArray::Create() {
		switch (Render3D::GetApi()) {
		case(RenderAPI::API::OpenGL): return std::make_shared<GLVertexArray>();//return new GLVertexArray{};
		}
		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

}
