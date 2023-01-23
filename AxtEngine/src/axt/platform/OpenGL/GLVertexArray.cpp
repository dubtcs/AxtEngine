#include <pch.h>

#include "GLVertexArray.h"

#include <glad/glad.h>

namespace axt {

	// TEMP
	static GLenum ShaderTypeToGLType(ShaderDataType type) {
		switch (type) {
		case (ShaderDataType::Float): return GL_FLOAT;
		case(ShaderDataType::Float2): return GL_FLOAT;
		case(ShaderDataType::Float3): return GL_FLOAT;
		case(ShaderDataType::Float4): return GL_FLOAT;

		case(ShaderDataType::Mat3): return GL_FLOAT;
		case(ShaderDataType::Mat4): return GL_FLOAT;

		case(ShaderDataType::Int): return GL_INT;
		case(ShaderDataType::Int2): return GL_INT;
		case(ShaderDataType::Int3): return GL_INT;

		case(ShaderDataType::Bool): return GL_BOOL;
		}
		AXT_CORE_ASSERT(false, "No ShaderDataType for GLenum");
		return GL_FALSE;
	}
	// TEMP

	GLVertexArray::GLVertexArray() {
		glGenVertexArrays(1, &id);
	}

	GLVertexArray::~GLVertexArray() {
		glDeleteVertexArrays(1, &id);
	}

	void GLVertexArray::Bind() const {
		glBindVertexArray(id);
	}

	void GLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void GLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		glBindVertexArray(id);
		vertexBuffer->Bind();
		uint32_t index{ 0 };
		const BufferLayout& layout{ vertexBuffer->GetLayout() };
		for (const BufferItem& item : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, item.GetItemCount(), ShaderTypeToGLType(item.type), item.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)item.offset); // make sure this is void*, mem address wont work
			index++;
		}
		this->vertexBuffers.push_back(vertexBuffer);
	}

	void GLVertexArray::AddIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		glBindVertexArray(id);
		indexBuffer->Bind();
		this->indexBuffer = indexBuffer;
	}

}