#include <pch.h>

#include "Framebuffer.h"

#include "RenderAPI.h"
#include "axt/platform/OpenGL/GLFrameBuffer.h"

namespace axt {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferData& newData) {
		switch (RenderAPI::GetAPI()) {
		case (RenderAPI::API::None): break;
		case(RenderAPI::API::OpenGL): return NewRef<GLFrameBuffer>(newData);
		}

		AXT_CORE_ASSERT(false, "No supported RenderAPI found!");
		return nullptr;
	}

}