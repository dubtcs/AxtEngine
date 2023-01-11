#pragma once

#include "axt/Core.h"
#include "Window.h"

#include "axt/events/Event.h"
#include "axt/events/WindowEvent.h"
#include "axt/events/KeyEvent.h"
#include "axt/events/MouseEvent.h"

#include "axt/core/Layer.h"
#include "axt/core/LayerStack.h"

#include "axt/gui/GuiLayer.h"

#include "axt/render/Buffers.h"
#include "axt/render/Shader.h"
#include "axt/render/VertexArray.h"

namespace axt {

	class AXT_API App {
	public:
		App();
		virtual ~App();
	public:
		void Run();
		bool OnEvent(Event& bindEvent);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		static App& GetApp() { return *instance; }
		AxtWindow& GetWindow() { return *window; }
	private:
		bool running{ true };
		bool OnWindowClose(WindowCloseEvent& ev);

		GuiLayer* guilayer;
		LayerStack layerstack;

		std::shared_ptr<AxtWindow> window;
		std::shared_ptr<VertexArray> vArray;
		//std::shared_ptr<VertexBuffer> vBuffer;
		//std::shared_ptr<IndexBuffer> iBuffer;
		std::shared_ptr<Shader> shader;

		// temp
		std::shared_ptr<Shader> squareShader;
		std::shared_ptr<VertexArray> squareArray;

		static App* instance;
	};

	App* CreateApp();

}