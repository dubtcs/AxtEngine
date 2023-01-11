
#include <pch.h>

#include "App.h"
#include "Log.h"

//temp
#include "axt/core/OpenShader.h"
#include "axt/core/Input.h"
#include "axt/render/Renderer.h"

namespace axt {

	App* App::instance{ nullptr };

	App::App() {
		AXT_CORE_INFO("AxtEngine starting.");
		AXT_ASSERT((instance == nullptr), "Application was not nullptr");
		instance = this;
		window = std::unique_ptr<AxtWindow>{ AxtWindow::Create() };
		window->SetVsync(true);
		window->SetEventCallback(std::bind(&App::OnEvent, this, std::placeholders::_1));
		guilayer = new GuiLayer{};
		PushOverlay(guilayer);

		vArray.reset(VertexArray::Create());

		float verts[7 * 3]{
			0.0f, .5f, 0.0f,   0.8f, 0.5f, 0.1f, 1.f,
			-0.5f, -0.5f, 0.0f, 1.f, 0.0f, 0.35f, 1.f,
			0.5f, -0.5f, 0.0f, 0.8f, 0.25f, 0.6f, 1.f
		};

		std::shared_ptr<VertexBuffer> vBuffer;
		vBuffer.reset(VertexBuffer::Create(verts, sizeof(verts)));
		{
			BufferLayout vLayout{
				{ShaderDataType::Float3, "inPos"},
				{ShaderDataType::Float4, "inColor"},
			};

			vBuffer->SetLayout(vLayout);
		}
		vArray->AddVertexBuffer(vBuffer);

		uint32_t ind[3]{ 0, 1, 2 }; // had this set to 1,2,3 ffs
		std::shared_ptr<IndexBuffer> iBuffer;
		iBuffer.reset(IndexBuffer::Create(ind, 3));
		vArray->AddIndexBuffer(iBuffer);

		float squareVertices[3 * 4]{
			0.5f, 0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			-0.5f, -0.5f, 0.f,
			-0.5f, 0.5f, 0.f
		};

		squareArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		{
			BufferLayout squareLayout{
				{ShaderDataType::Float3, "inPos"}
			};
			squareVB->SetLayout(squareLayout);
		}
		squareArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[]{ 0,1,2,0,2,3 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		squareArray->AddIndexBuffer(squareIB);

		// Shaders
		std::string vertexPath{ "shaders/v.vert" };
		std::string pixelPath{ "shaders/f.frag" };

		shader.reset(Shader::Create(OpenShader(vertexPath), OpenShader(pixelPath)));

		vertexPath = "shaders/square.vert";

		squareShader.reset(Shader::Create(OpenShader(vertexPath), OpenShader(pixelPath)));
	}

	App::~App() {

	}

	void App::Run() {

		glm::vec4 clearColor{ 0.15f, 0.15f, 0.15f, 1.0f };

		while (running) {
			RenderCommand::SetClearColor(clearColor);
			RenderCommand::Clear();

			Renderer::SceneStart();
			squareShader->Bind();
			Renderer::Submit(squareArray);
			shader->Bind();
			Renderer::Submit(vArray);
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
