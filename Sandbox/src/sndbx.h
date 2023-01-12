#pragma once

#include <AxtEngine.h>

class Sandbox : public axt::App {
public:
	Sandbox();
	~Sandbox() {};
};

class SandLayer : public axt::Layer {
	SandLayer(const std::string& name = "1738") : Layer(name) {};
	void OnUpdate() override { }
};
