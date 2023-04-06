#pragma once

#include <axt/Core.h>

#include "axt/ecs/nECS.h"

#include <map>

// just a wrapper for ecs

namespace axt {

	class AXT_API World
	{
	public:
		using ChildGroup = std::vector<ecs::EntityID>;

		ecs::EntityID CreateEntity(); // Parent to the world root
		ecs::EntityID CreateEntity(const ecs::EntityID& parent); // Parent to existing Entity

		void DestroyEntity(const ecs::EntityID& id);
		bool OnUpdate(float dt);

		template<typename T>
		T& GetComponent(const ecs::EntityID& id)
		{
			return mScene->GetComponent<T>(id);
		}

		template<typename T, typename... Ar>
		T& Attach(const ecs::EntityID& id, Ar&&... params)
		{
			return mScene->Attach<T>(id, std::forward<Ar>(params)...);
		}

		// THIS CREATES A COPY FROM INITIALIZER LIST OH NO
		template<typename T>
		T& Attach(ecs::EntityID& id, const T& copycat)
		{
			return mScene->Attach<T>(id, copycat);
		}

		template<typename T>
		void Detach(const ecs::EntityID& id)
		{
			return mScene->Detach<T>(id);
		}

		template<typename T>
		bool HasComponent(const ecs::EntityID& id)
		{
			return mScene->HasComponent<T>(id);
		}

		const ChildGroup& GetChildren(const ecs::EntityID& id)
		{
			return mHeirarchy[id].Children;
		}

		Ref<ecs::Scene>& GetScene()
		{
			return mScene;
		}
		const ecs::EntityID& GetRoot()
		{
			return mWorldRoot;
		}
	public:
		World();
		~World();
	protected:
		void CleanID(const ecs::EntityID& id);
	public:
		ecs::EntityID mActiveCamera;
	protected:
		struct Child
		{
			ecs::EntityID Parent;
			ChildGroup Children;
		};
		using Heirarchy = std::unordered_map<ecs::EntityID, Child>;

		Ref<ecs::Scene> mScene;
		ecs::EntityID mWorldRoot;
		Heirarchy mHeirarchy;
	};

}
