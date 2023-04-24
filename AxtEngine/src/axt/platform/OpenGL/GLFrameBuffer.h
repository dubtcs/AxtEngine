#pragma once

#include "axt/render/Framebuffer.h"

namespace axt {

	class GLFrameBuffer : public FrameBuffer {
	public:
		void Recreate();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetBufferID() const override;
		virtual uint32_t GetColorTextureID(const uint32_t index = 0) const override;
		virtual void Resize(uint32_t x, uint32_t y) override;
	public:
		GLFrameBuffer(const FrameBufferData& newData);
		virtual ~GLFrameBuffer();
	private:
		void Allocate();
		void Release();
	private:
		uint32_t mRenderId;
		uint32_t mDepthTexture;
		std::vector<uint32_t> mColorTextures;

		std::vector<FrameBufferTextureFormat> mColorTextureFormats;
		FrameBufferTextureFormat mDepthTextureFormat{ FrameBufferTextureFormat::None };
	};

}
