#include <pch.h>

#include "Texture.h"

#include "Renderer.h"
#include "axt/platform/OpenGL/GLTexture.h"

namespace axt {

	Ref<Texture2D> Texture2D::Create(uint32_t x, uint32_t y) {
		switch (Render3D::GetApi()) {
		case(RenderAPI::API::None): break;
		case(RenderAPI::API::OpenGL): return NewRef<GLTexture2D>(x, y);
		}

		AXT_CORE_ASSERT(false, "No render api found for Texture2D");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& filepath) {
		switch (Render3D::GetApi()) {
		case(RenderAPI::API::None): break;
		case(RenderAPI::API::OpenGL): return NewRef<GLTexture2D>(filepath);
		}

		AXT_CORE_ASSERT(false, "No render api found for Texture2D");
		return nullptr;
	}

	// LIBRARY

	void TextureLib::Add(const std::string& name, Ref<Texture2D>& texture) {
		if (mTextureMap.contains(name)) {
			AXT_CORE_WARN("Texture already exists!");
			return;
		}
		mTextureMap[name] = texture;
		return;
	}

	Ref<Texture2D> TextureLib::Get(const std::string& name) const {
		if (mTextureMap.contains(name)) {
			return mTextureMap.at(name);
		}
		AXT_CORE_WARN("No texture name exists!");
		return nullptr;
	}

	bool TextureLib::Contains(const std::string& name) const {
		return mTextureMap.contains(name);
	}

}