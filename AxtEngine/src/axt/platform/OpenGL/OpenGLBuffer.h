#pragma once

#include "axt/render/Buffers.h"

namespace axt{

	class AXT_API OGLVertexBuffer : public VertexBuffer {
	public:
		OGLVertexBuffer(float* vertices, size_t size);
		virtual ~OGLVertexBuffer() override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		unsigned int id;
	};

	class AXT_API OGLIndexBuffer : public IndexBuffer {
	public:
		OGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OGLIndexBuffer() override;
		virtual uint32_t GetCount() const override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint32_t count;
		unsigned int id;
	};

}
