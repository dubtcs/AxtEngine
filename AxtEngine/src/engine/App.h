#pragma once

#include "Core.h"

namespace axt {

	class AXT_API App {
	public:
		App();
		virtual ~App();
	public:
		void Run();
	};

	App* CreateApp();

}