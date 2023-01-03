#pragma once

#ifdef AXT_PLATFORM_WINDOWS

extern axt::App* axt::CreateApp();

int main(int argc, char** argv) {
	axt::Log::Init();
	AXT_CORE_INFO("Engine Start");
	AXT_INFO("Client Start");
	auto app{ axt::CreateApp() };
	app->Run();
	delete app;
}

#endif
