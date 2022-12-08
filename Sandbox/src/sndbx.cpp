
// client

#include <AxtEngine.h>

class Sndbx : public axt::App {

public:
	Sndbx() {};
	~Sndbx() {};

};

int main() {
	Sndbx* app = new Sndbx;
	app->Run();
	delete app;
}
