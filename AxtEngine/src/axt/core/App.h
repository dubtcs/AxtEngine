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

		std::unique_ptr<AxtWindow> window;
		std::unique_ptr<VertexBuffer> vBuffer;
		std::unique_ptr<IndexBuffer> iBuffer;
		std::unique_ptr<Shader> shader;

		static App* instance;

		//temp
		unsigned int vArray;
	};

	App* CreateApp();

}