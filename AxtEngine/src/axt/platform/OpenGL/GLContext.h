#pragma once

#include "axt/Core.h"
#include "axt/render/RenderContext.h"

struct GLFWwindow;

namespace axt {

	class AXT_API OpenGLContext : public RenderContext {
	public:
		OpenGLContext(GLFWwindow* window);
	public:
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* window;
	};

}
