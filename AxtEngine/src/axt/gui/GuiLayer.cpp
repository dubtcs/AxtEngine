
#include <pch.h>
#include "GuiLayer.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include "ImGuiBuilder.cpp"

#include "axt/core/App.h"

namespace axt {

	void SetCustomTheme();

	GuiLayer::GuiLayer() : Layer("GuiLayer") {
		
	}

	GuiLayer::~GuiLayer() {

	}

	void GuiLayer::OnAttach() {

		AXT_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io{ ImGui::GetIO() };
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("data/fonts/poppins/Poppins-Regular.ttf", 18.f);
		io.Fonts->AddFontFromFileTTF("data/fonts/poppins/Poppins-Bold.ttf", 18.f);

		//ImGui::StyleColorsDark();
		SetCustomTheme();
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
		AXT_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GuiLayer::Begin() {
		AXT_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void GuiLayer::End() {
		AXT_PROFILE_FUNCTION();

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

	/*void GuiLayer::OnImGuiRender() {
		static bool show{ true };
		ImGui::ShowDemoWindow(&show);
	}*/

	static const ImVec4 gBlackL1{ 0.05f, 0.05f, 0.05f, 1.f };
	static const ImVec4 gBlackL2{ 0.1f, 0.1f, 0.1f, 1.f };
	static const ImVec4 gBlackL3{ 0.15f, 0.15f, 0.15f, 1.f };
	static const ImVec4 gBlackL4{ 0.2f, 0.2f, 0.2f, 1.f };

	//static const ImVec4 gBlackL1OLED{ 0.f, 0.f, 0.f, 1.f };
	//static const ImVec4 gBlackL2OLED{ 0.05f, 0.05f, 0.05f, 1.f };
	//static const ImVec4 gBlackL3OLED{ 0.1f, 0.1f, 0.1f, 1.f };

	void SetCustomTheme()
	{
		ImGuiStyle& style{ ImGui::GetStyle() };
		auto& colors{ style.Colors };

		colors[ImGuiCol_WindowBg] = gBlackL2;

		colors[ImGuiCol_Header] = { gBlackL3 };
		colors[ImGuiCol_HeaderHovered] = { gBlackL4 };
		colors[ImGuiCol_HeaderActive] = { gBlackL4 };

		colors[ImGuiCol_Button] = { gBlackL2 };
		colors[ImGuiCol_ButtonHovered] = { gBlackL3 };
		colors[ImGuiCol_ButtonActive] = { gBlackL3 };

		colors[ImGuiCol_FrameBg] = { gBlackL3 };
		colors[ImGuiCol_FrameBgHovered] = { gBlackL4 };
		colors[ImGuiCol_FrameBgActive] = { gBlackL4 };

		colors[ImGuiCol_Tab] = { gBlackL2 };
		colors[ImGuiCol_TabHovered] = { gBlackL3 };
		colors[ImGuiCol_TabActive] = { gBlackL4 };
		colors[ImGuiCol_TabUnfocused] = { gBlackL2 };
		colors[ImGuiCol_TabUnfocusedActive] = { gBlackL3 };

		colors[ImGuiCol_TitleBg] = { gBlackL2 };
		colors[ImGuiCol_TitleBgActive] = { gBlackL2 };
		colors[ImGuiCol_TitleBgCollapsed] = { gBlackL1 };

	}

}
