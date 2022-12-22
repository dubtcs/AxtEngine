
#include <pch.h>
#include "GuiLayer.h"

#include <imgui.h>
#include "axt/App.h"

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
		EventHandler handler{ newEvent };
		handler.Fire<MouseButtonPressed>(AXT_BIND_EVENT(&GuiLayer::OnMouseButtonPressed));
		handler.Fire<MouseButtonReleased>(AXT_BIND_EVENT(&GuiLayer::OnMouseButtonRelease));
		handler.Fire<WindowResizeEvent>(AXT_BIND_EVENT(&GuiLayer::OnWindowResize));
		handler.Fire<MouseScrollEvent>(AXT_BIND_EVENT(&GuiLayer::OnMouseScroll));
		handler.Fire<MouseMovedEvent>(AXT_BIND_EVENT(&GuiLayer::OnMouseMoved));
		handler.Fire<KeyPressedEvent>(AXT_BIND_EVENT(&GuiLayer::OnKeyPressed));
		handler.Fire<KeyReleasedEvent>(AXT_BIND_EVENT(&GuiLayer::OnKeyReleased));
		handler.Fire<KeyTypedEvent>(AXT_BIND_EVENT(&GuiLayer::OnKeyTyped));
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

		AXT_INFO("GuiLayer Update");
	}

	bool GuiLayer::OnMouseButtonPressed(MouseButtonPressed& ev) {
		ImGuiIO& io{ ImGui::GetIO() };
		io.MouseDown[ev.GetButton()] = true;
		return false;
	}
	bool GuiLayer::OnMouseButtonRelease(MouseButtonReleased& ev) {
		ImGuiIO& io{ ImGui::GetIO() };
		io.MouseDown[ev.GetButton()] = false;
		return false;
	}

	bool GuiLayer::OnMouseMoved(MouseMovedEvent& ev) {
		ImGuiIO& io{ ImGui::GetIO() };
		io.MousePos = ImVec2{ (float)ev.GetX(), (float)ev.GetY() };
		return false;
	}

	bool GuiLayer::OnMouseScroll(MouseScrollEvent& ev) {
		ImGuiIO& io{ ImGui::GetIO() };
		io.MouseWheel += ev.GetY();
		io.MouseWheelH += ev.GetX();
		return false;
	}

	bool GuiLayer::OnWindowResize(WindowResizeEvent& ev) {
		ImGuiIO& io{ ImGui::GetIO() };
		io.DisplaySize = ImVec2{ (float)ev.GetWidth(), (float)ev.GetHegith() };
		io.DisplayFramebufferScale = ImVec2{ 1.f,1.f };
		return false;
	}

	bool GuiLayer::OnKeyPressed(KeyPressedEvent& ev) {
		ImGuiIO& io{ ImGui::GetIO() };
		io.KeysDown[ev.GetKeycode()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_RIGHT_CONTROL] || io.KeysDown[GLFW_KEY_LEFT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_RIGHT_SHIFT] || io.KeysDown[GLFW_KEY_LEFT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_RIGHT_ALT] || io.KeysDown[GLFW_KEY_LEFT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_RIGHT_SUPER] || io.KeysDown[GLFW_KEY_LEFT_SUPER];
		return false;
	}

	bool GuiLayer::OnKeyReleased(KeyReleasedEvent& ev) {
		ImGuiIO& io{ ImGui::GetIO() };
		io.KeysDown[ev.GetKeycode()] = false;
		return false;
	}

	bool GuiLayer::OnKeyTyped(KeyTypedEvent& ev) {
		ImGuiIO& io{ ImGui::GetIO() };
		int ch{ ev.GetKeycode() };
		if (ch > 0 && ch < 0x10000)
			io.AddInputCharacter((unsigned short)ch);
		return false;
	}

}
