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
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return vertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return indexBuffer; }
	private:
		unsigned int id;
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<IndexBuffer> indexBuffer;
	};

}
