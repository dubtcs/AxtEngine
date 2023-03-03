#include "pch.h"

#include "GLFrameBuffer.h"

#include <glad/glad.h>

namespace axt {

	GLFrameBuffer::GLFrameBuffer(const FrameBufferData& newData) : FrameBuffer{ newData } {
		Recreate();
	}

	GLFrameBuffer::~GLFrameBuffer() {
		glDeleteFramebuffers(1, &mRenderId);
		glDeleteTextures(1, &mColorTexture);
		//glDeleteTextures(1, &mDepthTexture);
		//glDeleteRenderbuffers(1, &mDepthBuffer);
	}

	void GLFrameBuffer::Recreate() {

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

#if 0
		glCreateFramebuffers(1, &mRenderId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRenderId);
		AXT_CORE_INFO(glCheckFramebufferStatus(GL_FRAMEBUFFER));

		// color
		// LOOK INTO GEN VS CREATE TEXTURES
		// this method wouldn't work
		glCreateTextures(GL_TEXTURE_2D, 1, &mColorTexture);
		glBindTexture(GL_TEXTURE_2D, mColorTexture);
		glTextureStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, mData.width, mData.height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// depth
#if 1
		glCreateTextures(GL_TEXTURE_2D, 1, &mDepthTexture);
		glBindTexture(GL_TEXTURE_2D, mDepthTexture);
		glTextureStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mData.width, mData.height);
		AXT_CORE_INFO(glCheckFramebufferStatus(GL_FRAMEBUFFER));

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);
		AXT_CORE_INFO(glCheckFramebufferStatus(GL_FRAMEBUFFER));
#endif

#if 0
		glCreateRenderbuffers(1, &mDepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mData.width, mData.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer);
#endif

		// assert complete
		AXT_CORE_INFO(glCheckFramebufferStatus(GL_FRAMEBUFFER));
		AXT_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer incomplete");

		// unbinding so no overwrites happen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

	}

	uint32_t GLFrameBuffer::GetBufferID() const {
		return mRenderId;
	}

	uint32_t GLFrameBuffer::GetColorTextureID() const {
		return mColorTexture;
	}

	void GLFrameBuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, mRenderId);
	}

	// static
	void GLFrameBuffer::Unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}