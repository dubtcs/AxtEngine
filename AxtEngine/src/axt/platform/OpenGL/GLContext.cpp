#include <pch.h>

#include "GLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace axt {

	OpenGLContext::OpenGLContext(GLFWwindow* newWindow) : window{ newWindow } {
		assert(newWindow != nullptr);
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(window);
		glfwMakeContextCurrent(window);
		int suc2{ gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) };
		assert(suc2 == 1);
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(window);
	}

}
