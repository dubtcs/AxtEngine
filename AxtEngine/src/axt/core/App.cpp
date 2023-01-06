
#include <pch.h>

#include "App.h"
#include "Log.h"

#include <glad/glad.h>

//temp
#include "axt/core/OpenShader.h"
#include "axt/core/Input.h"

namespace axt {

	App* App::instance{ nullptr };

	App::App() {
		assert(instance == nullptr);
		instance = this;
		window = std::unique_ptr<AxtWindow>{ AxtWindow::Create() };
		window->SetVsync(true);
		window->SetEventCallback(std::bind(&App::OnEvent, this, std::placeholders::_1));
		guilayer = new GuiLayer{};
		PushOverlay(guilayer);

		//temp
		glGenVertexArrays(1, &vArray);
		glBindVertexArray(vArray);

		float verts[3 * 3]{
			0.f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};

		vBuffer.reset(VertexBuffer::Create(verts, sizeof(verts)));

		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		uint32_t ind[3]{ 1, 2, 3 };
		iBuffer.reset(IndexBuffer::Create(ind, 3));

		std::string vertexPath{ "../AxtEngine/shaders/v.vert" };
		std::string pixelPath{ "../AxtEngine/shaders/f.frag" };

		shader.reset(Shader::Create(vertexPath, pixelPath));

	}

	App::~App() {

	}

	void App::Run() {
		while (running) {
			glClearColor(0.15f, 0.15f, 0.15f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			// temp
			shader->Bind();
			glBindVertexArray(vArray);
			glDrawElements(GL_TRIANGLES, iBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
