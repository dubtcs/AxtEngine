#pragma once

#include "axt/Core.h"

#include <string>

namespace axt {

	class AXT_API Texture {
	public:
		virtual ~Texture() = default;
		virtual void Bind(uint32_t textureSlot = 0) const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
	};

	class AXT_API Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const std::string& filepath);
	};

}
