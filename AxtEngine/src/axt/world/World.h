#pragma once

#include <axt/Core.h>

#include "axt/ecs/nECS.h"

// just a wrapper for ecs

namespace axt {

	class AXT_API World
	{
	public:
		ecs::EntityID CreateEntity();
		void DestroyEntity(const ecs::EntityID& id);
		bool OnUpdate(float dt);

		template<typename T>
		T& GetComponent(const ecs::EntityID& id)
		{
			return mScene->GetComponent<T>(id);
		}
		template<typename T>
		T& Attach(ecs::EntityID& id)
		{
			return mScene->Attach<T>(id);
		}
		template<typename T, typename... Ar>
		T& Attach(ecs::EntityID& id, Ar&&... params)
		{
			return mScene->Attach<T>(id, std::forward<Ar>(params));
		}
	public:
		World();
		~World();
	protected:
		Ref<ecs::Scene> mScene;
		ecs::EntityID mActiveCamera;
		// Camera vector?
	};

}
