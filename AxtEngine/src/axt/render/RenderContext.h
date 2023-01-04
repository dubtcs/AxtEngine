#pragma once

#include "axt/Core.h"

namespace axt {

	class AXT_API RenderContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	protected:
		RenderContext() {};
	};

}
