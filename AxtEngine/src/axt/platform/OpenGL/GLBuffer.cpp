#include <pch.h>

#include "GLBuffer.h"

#include <glad/glad.h>

namespace axt {

	/*****************/
	/* VERTEX BUFFER */
	/*****************/

	OGLVertexBuffer::OGLVertexBuffer(float* vertices, uint32_t size) {
		glCreateBuffers(1, &id);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OGLVertexBuffer::~OGLVertexBuffer() {
		glDeleteBuffers(1, &id);
	}

	void OGLVertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void OGLVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/****************/
	/* INDEX BUFFER */
	/****************/

	OGLIndexBuffer::OGLIndexBuffer(uint32_t* indices, uint32_t count) : count{ count } {
		glCreateBuffers(1, &id);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OGLIndexBuffer::~OGLIndexBuffer() {
		glDeleteBuffers(1, &id);
	}

	uint32_t OGLIndexBuffer::GetCount() const {
		return count;
	}

	void OGLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void OGLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}