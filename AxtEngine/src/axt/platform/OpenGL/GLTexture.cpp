#include <pch.h>

#include "GLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace axt {

	GLTexture2D::GLTexture2D(const std::string& filepath) {
		int x, y, channels;
		stbi_uc* data{ stbi_load(filepath.c_str(), &x, &y, &channels, 0) };
		AXT_CORE_ASSERT(data, "Image load failed");
		myWidth = x;
		myHeight = y;
		GLenum glFormat{ 0 }, dataFormat{ 0 };

		AXT_TRACE("{0}", channels);
		if (channels == 4) {
			glFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			glFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		AXT_CORE_ASSERT(glFormat & dataFormat, "Channels not available");

		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, glFormat, myWidth, myHeight);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureSubImage2D(id, 0, 0, 0, myWidth, myHeight, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data); // data pointer gone
	}

	GLTexture2D::~GLTexture2D() {
		glDeleteTextures(1, &id);
	}

	void GLTexture2D::Bind(uint32_t textureSlot) const {
		glBindTextureUnit(textureSlot, id);
	}

}