#include "pch.h"

#include "GLFrameBuffer.h"

#include <glad/glad.h>

namespace axt {

	GLFrameBuffer::GLFrameBuffer(const FrameBufferData& newData) : FrameBuffer{ newData } {
		Recreate();
	}

	GLFrameBuffer::~GLFrameBuffer() {
		Release();
	}

	void GLFrameBuffer::Resize(uint32_t x, uint32_t y) {
		mData.width = x;
		mData.height = y;
		Recreate();
	}

	void GLFrameBuffer::Recreate() {

		if (mRenderId) {
			Release();
		}

		glGenFramebuffers(1, &mRenderId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRenderId);

		// color
		glGenTextures(1, &mColorTexture);
		glBindTexture(GL_TEXTURE_2D, mColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mData.width, mData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture, 0);

		// depth
		glGenTextures(1, &mDepthTexture);
		glBindTexture(GL_TEXTURE_2D, mDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mData.width, mData.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

		AXT_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer incomplete");
		// unbinding so no overwrites happen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void GLFrameBuffer::Allocate() {
		glGenFramebuffers(1, &mRenderId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRenderId);
	}

	void GLFrameBuffer::Release() {
		glDeleteFramebuffers(1, &mRenderId);
		glDeleteTextures(1, &mColorTexture);
		glDeleteTextures(1, &mDepthTexture);
	}

	uint32_t GLFrameBuffer::GetBufferID() const {
		return mRenderId;
	}

	uint32_t GLFrameBuffer::GetColorTextureID() const {
		return mColorTexture;
	}

	void GLFrameBuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, mRenderId);
		glViewport(0, 0, mData.width, mData.height);
	}

	// static
	void GLFrameBuffer::Unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}