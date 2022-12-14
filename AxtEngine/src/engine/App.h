#pragma once

#include "Core.h"
#include "Window.h"

namespace axt {

	class AXT_API App {
	public:
		App();
		virtual ~App();
	public:
		void Run();
	private:
		bool running{ true };
		std::unique_ptr<AxtWindow> window;
	};

	App* CreateApp();

}