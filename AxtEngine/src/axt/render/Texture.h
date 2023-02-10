#pragma once

#include "axt/Core.h"

#include <string>

namespace axt {

	class AXT_API Texture {
	public:
		virtual ~Texture() = default;
		virtual void Bind(uint32_t textureSlot = 0) const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
	};

	class AXT_API Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(uint32_t x, uint32_t y);
		static Ref<Texture2D> Create(const std::string& filepath);
		virtual bool operator==(const Texture2D& other) const = 0;
	};

	class AXT_API TextureLib {
	public:
		void Add(const std::string& name, Ref<Texture2D>& texture);
		void Add(const std::string& name, const std::string& filepath);
		Ref<Texture2D> Get(const std::string& name) const;
		bool Contains(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Texture2D>> mTextureMap;
	};

}
