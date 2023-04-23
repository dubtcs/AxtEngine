#pragma once

#include <axt/Core.h>

#include "../render/EditorCamera.h"

#include <axt/world/GameWorld.h>
#include <axt/events/Event.h>

namespace axt
{

	class AXT_API EditorRender
	{
	public:
		EditorRender(float fieldOfView = 90);
		void OnUpdate(float dt, Ref<GameWorld>& world);
		bool OnEvent(Event& ev);
		void Init(float aspectRatio);
		EditorCamera GetCamera() const { return mCamera; }
	protected:
		EditorCamera mCamera{};
	};

}
