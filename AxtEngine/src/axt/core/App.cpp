
#include <pch.h>

#include "App.h"
#include "Log.h"

#include <glad/glad.h>

//temp
#include "axt/core/OpenShader.h"
#include "axt/core/Input.h"

namespace axt {

	App* App::instance{ nullptr };

	// TEMP
	static GLenum ShaderTypeToGLType(ShaderDataType type) {
		switch (type) {
		case (ShaderDataType::Float): return GL_FLOAT;
		case(ShaderDataType::Float2): return GL_FLOAT;
		case(ShaderDataType::Float3): return GL_FLOAT;
		case(ShaderDataType::Float4): return GL_FLOAT;

		case(ShaderDataType::Mat3): return GL_FLOAT;
		case(ShaderDataType::Mat4): return GL_FLOAT;

		case(ShaderDataType::Int): return GL_INT;
		case(ShaderDataType::Int2): return GL_INT;
		case(ShaderDataType::Int3): return GL_INT;

		case(ShaderDataType::Bool): return GL_BOOL;
		}
		AXT_CORE_ASSERT(false, "No ShaderDataType for GLenum");
		return GL_FALSE;
	}
	// TEMP

	App::App() {
		AXT_CORE_INFO("Engine Start");
		AXT_ASSERT((instance == nullptr), "Application was not nullptr");
		instance = this;
		window = std::unique_ptr<AxtWindow>{ AxtWindow::Create() };
		window->SetVsync(true);
		window->SetEventCallback(std::bind(&App::OnEvent, this, std::placeholders::_1));
		guilayer = new GuiLayer{};
		PushOverlay(guilayer);

		//temp
		glGenVertexArrays(1, &vArray);
		glBindVertexArray(vArray);

		/*float vertsold[3 * 3]{
			0.f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};*/

		float verts[7 * 3]{
			0.0f, .5f, 0.0f,   0.8f, 0.5f, 0.1f, 1.f,
			-0.5f, -0.5f, 0.0f, 1.f, 0.0f, 0.35f, 1.f,
			0.5f, -0.5f, 0.0f, 0.8f, 0.25f, 0.6f, 1.f
		};

		vBuffer.reset(VertexBuffer::Create(verts, sizeof(verts)));

		{
			BufferLayout vLayout{
				{ShaderDataType::Float3, "inPos"},
				{ShaderDataType::Float4, "inColor"},
			};

			vBuffer->SetLayout(vLayout);
		}

		uint32_t index{ 0 };
		const BufferLayout& layout{ vBuffer->GetLayout() };
		for (const BufferItem& item : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, item.GetItemCount(), ShaderTypeToGLType(item.type), item.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)item.offset); // make sure this is void*, mem address wont work
			index++;
		}

		uint32_t ind[3]{ 0, 1, 2 }; // had this set to 1,2,3 ffs
		iBuffer.reset(IndexBuffer::Create(ind, 3));

		std::string vertexPath{ "shaders/v.vert" };
		std::string pixelPath{ "shaders/f.frag" };

		shader.reset(Shader::Create(OpenShader(vertexPath), OpenShader(pixelPath)));
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
