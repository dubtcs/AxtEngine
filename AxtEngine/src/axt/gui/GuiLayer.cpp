
#include <pch.h>
#include "GuiLayer.h"

#include <imgui.h>
//#include <backends/imgui_impl_opengl3.h>
//#include <backends/imgui_impl_glfw.h>
#include "ImGuiBuilder.cpp"

#include "axt/core/App.h"

// temp
//#include <GLFW/glfw3.h>
//#include <glad/glad.h>

namespace axt {

	GuiLayer::GuiLayer() : Layer("GuiLayer") {
		
	}

	GuiLayer::~GuiLayer() {

	}

	void GuiLayer::OnAttach() {

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io{ ImGui::GetIO() };
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		App& App{ App::GetApp() };
		GLFWwindow* window{ static_cast<GLFWwindow*>(App.GetWindow().GetNativeWindow()) };

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void GuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GuiLayer::Begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GuiLayer::End() {
		ImGuiIO& io{ ImGui::GetIO() };
		App& app{ App::GetApp() };
		const AxtWindow& window{ app.GetWindow() };
		io.DisplaySize = ImVec2{ (float)window.GetWidth(), (float)window.GetHeight() };

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* contextBackup{ glfwGetCurrentContext() };
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(contextBackup);
		}
	}

	void GuiLayer::OnImGuiRender() {
		static bool show{ true };
		ImGui::ShowDemoWindow(&show);
	}

}
