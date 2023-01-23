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
#include "axt/render/Camera.h"

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
	protected:
		bool running{ true };
		bool OnWindowClose(WindowCloseEvent& ev);

		float lastFrameTime{ 0.f };

		GuiLayer* guilayer;
		LayerStack layerstack;

		Ref<AxtWindow> window; // may need witch back to shared ptr

		// for multiple renders
		std::vector<Ref<VertexArray>> vertexArrays;
		std::vector<Ref<Shader>> shaders;

		OrthoCamera myCamera;

		static App* instance;
	};

	App* CreateApp();

}