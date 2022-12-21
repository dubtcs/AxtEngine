
#include <pch.h>
#include "LayerStack.h"

namespace axt {

	LayerStack::LayerStack() {
		layerInsertPoint = layers.begin();
	}

	LayerStack::~LayerStack() {
		for (Layer* cur : layers) {
			delete cur;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		AXT_WARN("Push layer");
		layerInsertPoint = layers.emplace(layerInsertPoint, layer);
	}

	void LayerStack::PopLayer(Layer* layer) {
		AXT_WARN("Pooping out layer");
		std::vector<Layer*>::iterator iter{ std::find(layers.begin(), layers.end(), layer) };
		if (iter != layers.end()) {
			layers.erase(iter);
			layerInsertPoint--;
		}
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		layers.emplace_back(overlay);
		//layerInsertPoint++;
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		std::vector<Layer*>::iterator iter{ std::find(layers.begin(), layers.end(), overlay) };
		if (iter != layers.end()) {
			layers.erase(iter);
		}
	}

}
