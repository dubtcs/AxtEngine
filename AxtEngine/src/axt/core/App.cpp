
#include <pch.h>

#include "App.h"
#include "Log.h"

//temp
#include "axt/core/OpenShader.h"
#include "axt/core/Input.h"
#include "axt/render/Renderer.h"

#include "axt/render/Camera.h"

#include <glm/gtx/string_cast.hpp>

namespace axt {

	App* App::instance{ nullptr };

	App::App() : myCamera{-1.f, 1.f, -1.f, 1.f} {
		AXT_CORE_INFO("AxtEngine starting.");
		AXT_ASSERT((instance == nullptr), "Application was not nullptr");
		instance = this;
		window = std::unique_ptr<AxtWindow>{ AxtWindow::Create() };
		window->SetVsync(true);
		window->SetEventCallback(std::bind(&App::OnEvent, this, std::placeholders::_1));
		guilayer = new GuiLayer{};
		PushOverlay(guilayer);
	}

	App::~App() {

	}

	void App::Run() {

		glm::vec4 clearColor{ 0.15f, 0.15f, 0.15f, 1.0f };

		while (running) {
			RenderCommand::SetClearColor(clearColor);
			RenderCommand::Clear();

			std::string viewProjectionUniformName{ "uViewProjection" };

			const glm::mat4& viewProjection{ myCamera.GetViewProjection() };

			Renderer::SceneStart(myCamera);

			Renderer::Submit(squareArray, squareShader);
			Renderer::Submit(vArray, shader);

			Renderer::SceneEnd();

			for (Layer* curLayer : layerstack) {
				curLayer->OnUpdate();
			}

			guilayer->Begin();
			for (Layer* curLayer : layerstack) {
				curLayer->OnImGuiRender();
			}
			guilayer->End();

			window->Update();
		}
	}

	bool App::OnEvent(Event& bindEvent) {
		EventHandler handler{ bindEvent };
		handler.Fire<WindowCloseEvent>(std::bind(&App::OnWindowClose, this, std::placeholders::_1));

		// back to front for events
		for (std::vector<Layer*>::iterator iter{ layerstack.end() }; iter != layerstack.begin();) {
			(*--iter)->OnEvent(bindEvent);
			if (bindEvent.Handled()) {
				break;
			}
		}

		return true;
	}

	void App::PushLayer(Layer* layer) {
		layerstack.PushLayer(layer);
		layer->OnAttach();
	}

	void App::PushOverlay(Layer* overlay) {
		layerstack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool App::OnWindowClose(WindowCloseEvent& ev) {
		running = false;
		return true;
	}

}
