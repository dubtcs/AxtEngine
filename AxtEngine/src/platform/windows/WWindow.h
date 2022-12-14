#pragma once

#include <GLFW/glfw3.h>
#include "engine/Window.h"
#include "engine/events/Event.h"

namespace axt {

	class AXT_API WWindow : public AxtWindow {
	public:
		WWindow(const AxtWindowConfig& config = AxtWindowConfig{});
		~WWindow();
	public:
		void Update() override;
		void SetVsync(bool toggle) override;
		void SetEventCallback(std::function<bool(Event&)> bindFunction) override;
		bool IsVsync() const override;
		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;
		static void GlfwErrorCallback(int er, const char* desc);
	private:
		void Init();
		void Shutdown();
	protected:

		struct WindowData {
			unsigned int width, height;
			std::string title;
			bool vsync{ true };
			std::function<bool(Event&)> callback;
			WindowData(const AxtWindowConfig& config = AxtWindowConfig{}) : width{ config.width }, height{ config.height }, title{ config.title } {};
		};

		WindowData data;
		GLFWwindow* window;
	};

}
