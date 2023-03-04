#pragma once

#include "axt/Core.h"

namespace axt {

	struct AXT_API FrameBufferData {
		uint32_t width{ 1920 }, height{ 1080 };
		bool existsInSwapChain{ false };
	};

	class AXT_API FrameBuffer {
	public:
		static Ref<FrameBuffer> Create(const FrameBufferData& newData = FrameBufferData{});
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetBufferID() const = 0;
		virtual uint32_t GetColorTextureID() const = 0;
		virtual void Resize(uint32_t x, uint32_t y) = 0;
		virtual ~FrameBuffer() = default;
	protected:
		FrameBuffer(const FrameBufferData& newData) : mData{ newData } {}
		FrameBufferData mData;
	};

}
