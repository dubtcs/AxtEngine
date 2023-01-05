
#include <pch.h>

#include "App.h"
#include "Log.h"

#include <glad/glad.h>

//temp
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

		glGenBuffers(1, &vBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vBuffer);

		float verts[3 * 3]{
			0.f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &iBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);

		unsigned int ind[3]{ 1,2,3 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

		std::string vSrc{ R"(
			#version 330 core

			layout (location = 0) in vec3 inPos;
			out vec3 fPosition;

			void main(){
				gl_Position = vec4(inPos, 1.0);
				fPosition = inPos;
			}
		)" };

		std::string fSrc{ R"(
			#version 330 core

			out vec4 outColor;
			in vec3 fPosition;

			void main(){
				outColor = vec4(fPosition + 0.5, 1.0);
			}
		)" };

		shader.reset(new Shader{vSrc, fSrc});

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
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
