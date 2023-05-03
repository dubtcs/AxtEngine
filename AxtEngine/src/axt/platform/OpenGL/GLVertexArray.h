#pragma once

#include "axt/Core.h"

#include "axt/render/VertexArray.h"

#include <memory>
#include <vector>

namespace axt {

	class AXT_API GLVertexArray : public VertexArray {
	public:
		GLVertexArray();
		virtual ~GLVertexArray() override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
		virtual void ToggleVertexBuffer(const Ref<VertexBuffer>& buffer) override;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return vertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return indexBuffer; }
	private:
		unsigned int id;
		std::vector<Ref<VertexBuffer>> vertexBuffers;
		Ref<IndexBuffer> indexBuffer;
	};

}
