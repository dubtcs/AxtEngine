#pragma once

#include <pch.h>
#include "axt/Core.h"
#include "axt/core/Layer.h"

namespace axt {

	class AXT_API LayerStack {
	public:
		LayerStack();
		~LayerStack();
		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);
		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }
	protected:
		std::vector<Layer*> layers{};
		unsigned int layerInsertIndex{ 0 };
	};

}
