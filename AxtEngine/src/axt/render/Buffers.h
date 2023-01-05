#pragma once

#include "axt/Core.h"

namespace axt {

	class AXT_API VertexBuffer {
	public:
		virtual ~VertexBuffer() {};
		static VertexBuffer* Create(float* vertices, size_t size);
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class AXT_API IndexBuffer {
	public:
		virtual ~IndexBuffer() {};
		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	public:
		virtual uint32_t GetCount() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

}
