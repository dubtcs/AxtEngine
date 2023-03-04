#pragma once

// ONLY INCLUDE ONCE IN THE MAIN APP FILE

#ifdef AXT_PLATFORM_WINDOWS

extern axt::App* axt::CreateApp();

int main(int argc, char** argv) {
	axt::Log::Init();
	AXT_INFO("Entrypoint created.");
	AXT_PROFILE_NEW_SESSION("Startup", "PROFILE-STARTUP.json");
	auto app{ axt::CreateApp() };
	AXT_PROFILE_END_SESSION();
	AXT_PROFILE_NEW_SESSION("Runtime", "PROFILE-RUNTIME.json");
	app->Run();
	AXT_PROFILE_END_SESSION();
	AXT_PROFILE_NEW_SESSION("Shutdown", "PROFILE-SHUTDOWN.json");
	delete app;
	AXT_PROFILE_END_SESSION();
}

#endif
