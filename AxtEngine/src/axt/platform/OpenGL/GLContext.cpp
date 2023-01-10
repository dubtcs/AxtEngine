#include <pch.h>

#include "GLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace axt {

	OpenGLContext::OpenGLContext(GLFWwindow* newWindow) : window{ newWindow } {
		AXT_CORE_ASSERT((newWindow != nullptr), "Window was not created");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(window);
		glfwMakeContextCurrent(window);
		int suc2{ gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) };
		AXT_CORE_ASSERT((suc2 == 1), "OpenGL headers could not be loaded");

		const char* const infodump{ "Using OpenGL - Device Info:\n\tVendor: {0}\n\tDevice: {1}\n\tVersion: {2}" };

		AXT_CORE_INFO(infodump, (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER), (const char*)glGetString(GL_VERSION));

	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(window);
	}

}
