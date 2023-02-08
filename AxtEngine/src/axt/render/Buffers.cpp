#include <pch.h>

#include "Buffers.h"

#include "axt/render/Renderer.h"

#include "axt/platform/OpenGL/GLBuffer.h"

namespace axt {

	uint32_t BufferItem::GetItemCount() const {
		switch (type) {
		case (ShaderDataType::Float): return 1;
		case(ShaderDataType::Float2): return 2;
		case(ShaderDataType::Float3): return 3;
		case(ShaderDataType::Float4): return 4;

		case(ShaderDataType::Mat3): return 9;
		case(ShaderDataType::Mat4): return 16;

		case(ShaderDataType::Int): return 1;
		case(ShaderDataType::Int2): return 2;
		case(ShaderDataType::Int3): return 3;

		case(ShaderDataType::Bool): return 1;
		}
		AXT_CORE_ASSERT(false, "No ShaderDataType found.");
		return 0;
	}

	void BufferLayout::CalculateStride() {
		int currentOffset{ 0 };
		itemStride = 0;
		for (BufferItem& currentItem : items) {
			currentItem.offset = currentOffset;
			uint32_t& size{ currentItem.size };
			itemStride += size;
			currentOffset += size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Render3D::GetApi()) {
			case(RenderAPI::API::None): break;
			case(RenderAPI::API::OpenGL): return NewRef<OGLVertexBuffer>(vertices, size);//return (new OGLVertexBuffer{ vertices, size });
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch (Render3D::GetApi()) {
			case(RenderAPI::API::None): break;
			case(RenderAPI::API::OpenGL): return NewRef<OGLVertexBuffer>(size);//return (new OGLVertexBuffer{ vertices, size });
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Render3D::GetApi()) {
		case(RenderAPI::API::None): break;
		case(RenderAPI::API::OpenGL): return NewRef<OGLIndexBuffer>(indices, size);//return (new OGLIndexBuffer{ indices, size });
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

}
