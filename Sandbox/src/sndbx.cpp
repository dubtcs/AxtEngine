
// client

#include <AxtEngine.h>

class Sandbox : public axt::App {

public:
	Sandbox() {};
	~Sandbox() {};
};

axt::App* axt::CreateApp() {
	return new Sandbox;
}
