#pragma once

#include <axt/Core.h>

#include <necs/include.h>

namespace axt
{

	// All game objects have a heirarchy REQUIRED

	class AXT_API GameWorld
	{
	public:
		necs::Entity CreateEntity(const necs::Entity& parent);
		void DestroyEntity(const necs::Entity& id);
		bool ChangeParent(const necs::Entity& id, const necs::Entity& newParent);
		bool AddChild(const necs::Entity& parent, const necs::Entity& child);
		Ref<necs::Scene> GetScene() { return mScene; }
	public:
		GameWorld();
	protected:
		Ref<necs::Scene> mScene;
		necs::Entity mWorldRoot;

	public:

		// dont use these for now
		template<typename T>
		T& Attach(const necs::Entity& id, const T& other)
		{
			return mScene->Attach<T>(id, other);
		}

		// dont use these for now
		template<typename T, typename... Ar>
		T& Attach(const necs::Entity& id, Ar... params)
		{
			return mScene->Attach<T>(id, params...);
		}

		// dont use these for now
		template<typename T>
		void Detach(const necs::Entity& id)
		{
			return mScene->Detach<T>(id);
		}

	};

}
