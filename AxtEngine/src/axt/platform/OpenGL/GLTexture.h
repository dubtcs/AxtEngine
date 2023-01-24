#pragma once

#include "axt/render/Texture.h"

namespace axt {

	class AXT_API GLTexture : public Texture {

	};

	class AXT_API GLTexture2D : public Texture2D {
	public:
		GLTexture2D(const std::string& filepath);
		~GLTexture2D();
	public:
		virtual void Bind(uint32_t textureSlot = 0) const override;
		virtual uint32_t GetWidth() const override { return myWidth; };
		virtual uint32_t GetHeight() const override { return myHeight; };
	protected:
		uint32_t myWidth, myHeight, id;
	};

}
