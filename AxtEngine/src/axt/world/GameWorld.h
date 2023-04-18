#pragma once

#include <axt/Core.h>

#include "RenderSystem.h"
#include "CameraControlSystem.h"

#include <necs/include.h>

namespace axt
{

	// All game objects have a heirarchy REQUIRED

	class AXT_API GameWorld
	{
	public:
		necs::Entity CreateEntity(const necs::Entity& parent);
		void DestroyEntity(const necs::Entity& id);
		Ref<necs::Scene> GetScene() { return mScene; }
	public:
		GameWorld();
	protected:
		Ref<necs::Scene> mScene;
		necs::Entity mWorldRoot;
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
