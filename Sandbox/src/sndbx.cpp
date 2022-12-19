
// client

#include <AxtEngine.h>

class ELayer : public axt::Layer {
public:
	ELayer() : Layer("Bruh") {};
	void OnUpdate() override { }
	void OnEvent(axt::Event& event) override { AXT_TRACE(event.ToString()); }
};

class Sandbox : public axt::App {
public:
	Sandbox() { PushLayer(new ELayer{}); };
	~Sandbox() {};
};

axt::App* axt::CreateApp() {
	return new Sandbox;
}
