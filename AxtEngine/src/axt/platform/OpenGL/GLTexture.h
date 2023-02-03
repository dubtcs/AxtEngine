#pragma once

#include "axt/render/Texture.h"

#include <glad/glad.h>

namespace axt {

	class AXT_API GLTexture : public Texture {

	};

	class AXT_API GLTexture2D : public Texture2D {
	public:
		GLTexture2D(uint32_t x, uint32_t y);
		GLTexture2D(const std::string& filepath);
		~GLTexture2D();
	public:
		virtual void Bind(uint32_t textureSlot = 0) const override;
		virtual void SetData(void* data, uint32_t size) override;
		virtual uint32_t GetWidth() const override { return mWidth; };
		virtual uint32_t GetHeight() const override { return mHeight; };
	protected:
		uint32_t mWidth, mHeight, id;
		GLenum mGlFormat, mDataFormat;
	};

}
