#include "pch.h"

#include "GLFrameBuffer.h"

#include <glad/glad.h>

static constexpr uint32_t gMaxDrawBuffers{ 2 };

namespace axt {

	// FrameBuffer util
	namespace fbutil
	{

		static bool IsDepthTextureFormat(const FrameBufferTextureFormat& dataType)
		{
			switch (dataType)
			{
				case(FrameBufferTextureFormat::DEPTH24STENCIL8) :
				{
					return true;
				}
			}
			return false;
		}

		static void ReserveFrameBufferTextures(uint32_t* startAddress, uint32_t amount = 1)
		{
			glGenTextures(amount, startAddress);
		}

		static void CreateColorTexture(uint32_t& id, uint32_t offset, FrameBufferTextureFormat& format, uint32_t& width, uint32_t& height)
		{
			glBindTexture(GL_TEXTURE_2D, id);

			switch (format)
			{
				case (FrameBufferTextureFormat::RGBA8) :
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				}
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + offset, GL_TEXTURE_2D, id, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		static void CreateDepthTexture(uint32_t& id, uint32_t offset, FrameBufferTextureFormat& format, uint32_t& width, uint32_t& height)
		{
			glBindTexture(GL_TEXTURE_2D, id);
			switch (format)
			{
				case (FrameBufferTextureFormat::DEPTH24STENCIL8) : 
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
				}
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, id, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}

	GLFrameBuffer::GLFrameBuffer(const FrameBufferData& newData) : 
		FrameBuffer{ newData } 
	{
		for (FrameBufferTextureData& data : mData.Textures.TextureData)
		{
			if (fbutil::IsDepthTextureFormat(data.Format))
			{
				mDepthTextureFormat = data.Format;
			}
			else
			{
				mColorTextureFormats.push_back(data.Format);
			}
		}
		Recreate();
	}

	GLFrameBuffer::~GLFrameBuffer() 
	{
		Release();
	}

	void GLFrameBuffer::Resize(uint32_t x, uint32_t y) 
	{
		mData.Width = x;
		mData.Height = y;
		Recreate();
	}

	void GLFrameBuffer::Recreate() 
	{

		if (mRenderId) 
		{
			Release();
		}

		glGenFramebuffers(1, &mRenderId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRenderId);

		// Color
		{
			if (mColorTextureFormats.size() > 0)
			{
				mColorTextures.resize(mColorTextureFormats.size());
				fbutil::ReserveFrameBufferTextures(&mColorTextures.front(), mColorTextureFormats.size());

				for (uint32_t i{ 0 }; i < mColorTextureFormats.size(); i++)
				{
					fbutil::CreateColorTexture(mColorTextures[i], i, mColorTextureFormats[i], mData.Width, mData.Height);
				}
			}
		}

		// Depth
		if (mDepthTextureFormat != FrameBufferTextureFormat::None)
		{
			fbutil::ReserveFrameBufferTextures(&mDepthTexture);
			fbutil::CreateDepthTexture(mDepthTexture, 0, mDepthTextureFormat, mData.Width, mData.Height);
		}

		if (mColorTextures.size() > 1)
		{
			GLenum buffers[gMaxDrawBuffers]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(gMaxDrawBuffers, buffers);
			// this affects global FBO state, don't need to call this every frame
		}


		AXT_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer incomplete");

		// unbinding so no overwrites happen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void GLFrameBuffer::Allocate() 
	{
		glGenFramebuffers(1, &mRenderId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRenderId);
	}

	void GLFrameBuffer::Release() 
	{
		glDeleteFramebuffers(1, &mRenderId);
		if (!mColorTextures.empty())
		{
			glDeleteTextures(mColorTextures.size(), &mColorTextures.front());
			mColorTextures.clear();
		}
		glDeleteTextures(1, &mDepthTexture);
		mDepthTexture = 0;
	}

	uint32_t GLFrameBuffer::GetBufferID() const 
	{
		return mRenderId;
	}

	uint32_t GLFrameBuffer::GetColorTextureID(const uint32_t index) const 
	{
		return mColorTextures[index];
	}

	void GLFrameBuffer::Bind() const 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRenderId);
		glViewport(0, 0, mData.Width, mData.Height);
	}

	// static
	void GLFrameBuffer::Unbind() const 
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}