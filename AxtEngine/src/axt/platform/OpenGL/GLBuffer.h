#pragma once

#include "axt/render/Buffers.h"

namespace axt{

	class AXT_API OGLVertexBuffer : public VertexBuffer {
	public:
		OGLVertexBuffer(float* vertices, uint32_t size);
		OGLVertexBuffer(uint32_t size);
		virtual ~OGLVertexBuffer() override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetLayout(const BufferLayout& newLayout) override { layout = newLayout; }
		virtual BufferLayout& GetLayout() override { return layout; };
		virtual void SubmitData(const void* data, uint32_t size, uint32_t offset = 0) const override;
	private:
		uint32_t id;
		BufferLayout layout{};
	};

	class AXT_API OGLIndexBuffer : public IndexBuffer {
	public:
		OGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OGLIndexBuffer() override;
		virtual uint32_t GetCount() const override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint32_t count;
		unsigned int id;
	};

}
