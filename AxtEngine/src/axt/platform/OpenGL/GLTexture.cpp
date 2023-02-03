#include <pch.h>

#include "GLTexture.h"

#include <stb_image.h>

namespace axt {

	GLTexture2D::GLTexture2D(uint32_t x, uint32_t y) : mWidth{ x }, mHeight{ y } {
		mGlFormat = GL_RGBA8;
		mDataFormat = GL_RGBA;
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, mGlFormat, mWidth, mHeight);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	GLTexture2D::GLTexture2D(const std::string& filepath) {
		int x, y, channels;
		stbi_uc* data{ stbi_load(filepath.c_str(), &x, &y, &channels, 0) };
		AXT_CORE_ASSERT(data, "Image load failed");
		mWidth = x;
		mHeight = y;
		//GLenum glFormat{ 0 }, dataFormat{ 0 };

		if (channels == 4) {
			mGlFormat = GL_RGBA8;
			mDataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			mGlFormat = GL_RGB8;
			mDataFormat = GL_RGB;
		}
		AXT_CORE_ASSERT(mGlFormat & mDataFormat, "Channels not available");

		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, mGlFormat, mWidth, mHeight);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureSubImage2D(id, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data); // data pointer gone
	}

	void GLTexture2D::SetData(void* data, uint32_t size) {
		AXT_CORE_ASSERT((size == (mWidth * mHeight * (mDataFormat == GL_RGBA ? 4 : 3))), "GLTexture SetData Size must be equal to the entire texture.");
		glTextureSubImage2D(id, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);
	}

	GLTexture2D::~GLTexture2D() {
		glDeleteTextures(1, &id);
	}

	void GLTexture2D::Bind(uint32_t textureSlot) const {
		glBindTextureUnit(textureSlot, id);
	}

}