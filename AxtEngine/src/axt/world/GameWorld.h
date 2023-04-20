#pragma once

#include <axt/Core.h>

#include "UUID.h"

#include <necs/include.h>

namespace axt
{

	// All game objects have a GraphData REQUIRED

	struct AXT_API EntityData
	{
		necs::Entity entity;
		axt::UUID id;
	};

	class AXT_API GameWorld
	{
	public:
		GameWorld();
		GameWorld(const axt::UUID& worldRootID);
	protected:
		void CreateRootNode(const axt::UUID& id);
	public:
		necs::Entity CreateEntity();
		necs::Entity CreateEntity(const necs::Entity& parent);
		necs::Entity CreateEntityWithUUID(const necs::Entity& parent, const axt::UUID& id);

		// Load an existing entity from an axt scene file
		necs::Entity LoadEntity(const axt::UUID& parent, const axt::UUID& id);

		void DestroyEntity(const necs::Entity& id);

		void SetActiveCamera(const necs::Entity& id) { mActiveCamera = id; }

		necs::Entity GetEntityFromUUID(const axt::UUID& id) { return mMap[id]; }
		necs::Entity GetActiveCamera() const { return mActiveCamera; }
		const necs::Entity& GetWorldRoot() { return mWorldRoot; }
		Ref<necs::Scene>& GetScene() { return mScene; }
	protected:
		Ref<necs::Scene> mScene;
		necs::Entity mWorldRoot;
		necs::Entity mActiveCamera;

		bool mRootCreated{ false };

		using UUIDMap = std::unordered_map<UUID, necs::Entity>;
		UUIDMap mMap{};
	public:

		// Attach a component to an entity
		template<typename T>
		T& Attach(const necs::Entity& entity)
		{
			return mScene->Attach<T>(entity);
		}

		/*
		@brief Attach a component to an entity
		@param entity: Entity id
		@param other: object of type T to set as initial value
		*/
		template<typename T>
		T& Attach(const necs::Entity& entity, const T& other)
		{
			return mScene->Attach<T>(entity, other);
		}

		/*
		@brief Attach a component to an entity
		@param entity: Entity id
		@param initializer_list: Initializer list of type T to set as initial value
		*/
		template<typename T>
		T& Attach(const necs::Entity& entity, std::initializer_list<T>&& initializer_list)
		{
			return mScene->Attach<T>(entity, std::forward<std::initializer_list<T>>(initializer_list));
		}



		// Removes a component instance from the entity
		template<typename T>
		void Detach(const necs::Entity& entity)
		{
			return mScene->Detach<T>(entity);
		}



		// Check if an entity has a component attached
		template<typename T>
		bool HasComponent(const necs::Entity& entity)
		{
			return mScene->HasComponent<T>(entity);
		}



		/*
		@brief Get the component data attached to the entity
		@brief Does *NOT* check if component is attached. Use HasComponent<T>(entity) before calling this
		*/
		template<typename T>
		T& GetComponent(const necs::Entity& entity)
		{
			return mScene->GetComponent<T>(entity);
		}

	};

}
