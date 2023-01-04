
#include <pch.h>
#include "LayerStack.h"

namespace axt {

	LayerStack::LayerStack() {

	}

	LayerStack::~LayerStack() {
		for (Layer* cur : layers) {
			delete cur;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		AXT_WARN("Push layer");
		layers.emplace(layers.begin() + layerInsertIndex, layer);
		layerInsertIndex++;
	}

	void LayerStack::PopLayer(Layer* layer) {
		AXT_WARN("Pooping out layer");
		std::vector<Layer*>::iterator iter{ std::find(layers.begin(), layers.end(), layer) };
		if (iter != layers.end()) {
			layers.erase(iter);
			layerInsertIndex--;
		}
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		layers.emplace_back(overlay);
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		std::vector<Layer*>::iterator iter{ std::find(layers.begin(), layers.end(), overlay) };
		if (iter != layers.end()) {
			layers.erase(iter);
		}
	}

}
