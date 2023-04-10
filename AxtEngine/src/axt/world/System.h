#pragma once

#include <axt/Core.h>

#include<necs/include.h>

namespace axt
{

	class AXT_API System
	{
	protected:
		Ref<necs::Scene> mScene;
		System(Ref<necs::Scene>& scene) : mScene{ scene } {};
		~System() = default;
	};

}
