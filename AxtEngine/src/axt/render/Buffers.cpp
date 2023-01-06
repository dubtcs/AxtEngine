#include <pch.h>

#include "Buffers.h"

#include "axt/render/Renderer.h"
#include "axt/platform/OpenGL/GLBuffer.h"

namespace axt {

	VertexBuffer* VertexBuffer::Create(float* vertices, size_t size) {
		switch (Renderer::GetApi()) {
		case(RenderAPI::None): return nullptr;
		case(RenderAPI::OpenGL) : return (new OGLVertexBuffer{ vertices, size });
		}

		assert(false); // no render API found
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Renderer::GetApi()) {
		case(RenderAPI::None):return nullptr;
		case(RenderAPI::OpenGL):return (new OGLIndexBuffer{ indices, size });
		}

		assert(false); // no render api found
		return nullptr;
	}

}
