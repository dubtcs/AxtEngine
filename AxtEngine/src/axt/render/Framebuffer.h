#pragma once

#include "axt/Core.h"

#include <vector>
#include <cstdint>

namespace axt {

	// I could possible move these into the FrameBuffer class
	// so formatting could look like
	// FrameBuffer::TextureFormat
	// FrameBuffer::TextureData
	// FrameBuffer::TextureCollection etc...

	enum class FrameBufferTextureFormat
	{
		None = 0,

		RGBA8,
		R_INTEGER,

		DEPTH24STENCIL8,

		//Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureData
	{
		FrameBufferTextureFormat Format{ FrameBufferTextureFormat::None };
		FrameBufferTextureData() = default;
		FrameBufferTextureData(const FrameBufferTextureFormat& format) : Format{ format } {}
	};

	struct FrameBufferTextureCollection
	{
		std::vector<FrameBufferTextureData> TextureData;
		FrameBufferTextureCollection() = default;
		FrameBufferTextureCollection(const std::initializer_list<FrameBufferTextureData>& textureData) : TextureData{ textureData } {}
	};

	struct AXT_API FrameBufferData 
	{
		uint32_t Width{ 1920 };
		uint32_t Height{ 1080 };

		FrameBufferTextureCollection Textures;

		//bool ExistsInSwapChain{ false };
	};

	class AXT_API FrameBuffer 
	{
	public:
		static Ref<FrameBuffer> Create(const FrameBufferData& newData = FrameBufferData{});
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetBufferID() const = 0;
		virtual uint32_t GetColorTextureID(const uint32_t index = 0) const = 0;
		virtual uint32_t GetPixelData(const uint32_t& textureIndex, int32_t x, int32_t y) const = 0;
		virtual void Resize(uint32_t x, uint32_t y) = 0;
		virtual ~FrameBuffer() = default;
	protected:
		FrameBuffer(const FrameBufferData& newData) : mData{ newData } {}
		FrameBufferData mData;
	};

}
