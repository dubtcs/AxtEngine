
#include <pch.h>
#include "GuiLayer.h"

#include <imgui.h>
#include "engine/App.h"

#include "platform/OpenGL/IGOGLRender.h"

#include <GLFW/glfw3.h>

namespace axt {

	GuiLayer::GuiLayer() : Layer("GuiLayer") {

	}

	GuiLayer::~GuiLayer() {

	}

	void GuiLayer::OnAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGuiIO& io{ ImGui::GetIO() };
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Temp
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_LeftShift] = GLFW_KEY_LEFT_SHIFT;
		io.KeyMap[ImGuiKey_W] = GLFW_KEY_W;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_S] = GLFW_KEY_S;
		io.KeyMap[ImGuiKey_D] = GLFW_KEY_D;
		
		ImGui_ImplOpenGL3_Init("version 410");

	}

	void GuiLayer::OnDetach() {

	}

	void GuiLayer::OnEvent(Event& newEvent) {

	}

	void GuiLayer::OnUpdate() {
		ImGuiIO& io{ ImGui::GetIO() };

		const AxtWindow& curWindow{ App::GetApp().GetWindow() };
		io.DisplaySize = ImVec2{ (float)curWindow.GetWidth(), (float)curWindow.GetHeight() };

		float dt{ (float)glfwGetTime() };
		io.DeltaTime = previousTime > 0.f ? (dt - previousTime) : (1.f / 144.f);
		previousTime = dt;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool showWindiow{ true };
		ImGui::ShowDemoWindow(&showWindiow);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}
