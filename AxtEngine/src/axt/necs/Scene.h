#pragma once

#include "etypes.h"
#include "CPack.h"
#include "IDHandler.h"

namespace necs
{

	class Scene
	{
	public:
		Scene() : mIDManager{} {};

		Entity CreateEntity()
		{
			return mIDManager.Create();
		}

		void DestroyEntity(const Entity& id)
		{
			mIDManager.Destroy(id);
			mSignatures.at(id).reset();
			for (ComponentTypeID i{ 0 }; i < GComponentTypesRegistered; i++)
			{
				if (mPacks.at(i) != nullptr)
				{
					mPacks.at(i)->Destroy(id);
				}
			}
		}

		template<typename T, typename... Ar>
		T& Attach(const Entity& id, Ar... params)
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };
			if (mPacks.size() <= cid)
			{
				mPacks.resize(cid);
				mPacks.push_back(MakeShared<ComponentPack<T>>());
			}

			if (mSignatures.size() <= id)
			{
				mSignatures.resize(id);
			}
			mSignatures.push_back(0);
			mSignatures.at(id).set(cid);

			Shared<ComponentPack<T>> pack{ std::static_pointer_cast<ComponentPack<T>>(mPacks.at(cid)) };
			pack->Add(id, std::forward<Ar>(params)...);
			return pack->Get(id);
		}

		template<typename T>
		void Detach(const Entity& id)
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };
			if (mPacks.size() <= cid)
				return;

			mSignatures.at(id).reset(cid);
			Shared<ComponentPack<T>> pack{ std::static_pointer_cast<ComponentPack<T>>(mPacks.at(cid)) };
			pack->Destroy(id);
		}

		template<typename T>
		T& Get(const Entity& id)
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };
			if (mPacks.size() <= cid)
				assert(false && "Pack no exists honey");
			return (std::static_pointer_cast<ComponentPack<T>>(mPacks.at(cid)))->Get(id);
		}

		template<typename T>
		bool HasAttached(const Entity& id)
		{
			ComponentTypeID cid{ GetComponentTypeID<T>() };
			if (mPacks.size() <= cid)
				assert(false && "Pack no exists honey");
			return mSignatures->at(id).Mask.test(cid);
		}

		std::vector<Signature>& GetSignatures()
		{
			return mSignatures;
		}

		IDHandler& GetIDHandler()
		{
			return mIDManager;
		}

	protected:
		std::vector<Shared<AComponentPack>> mPacks;
		std::vector<Signature> mSignatures;
		IDHandler mIDManager;
	};

	template<typename... T>
	class SceneView
	{
	public:
		SceneView(Shared<Scene>& scene)
		{
			static_assert(sizeof...(T) > 0, "Must have at least 1 component in scene view");

			ComponentTypeID ids[]{ GetComponentTypeID<T>()... };
			for (ComponentTypeID i : ids)
			{
				mSignature.set(i);
			}

			const std::vector<Signature>& sigs{ scene->GetSignatures() };
			for (Entity i : scene->GetIDHandler())
			{
				if ((mSignature & (sigs.at(i))) == mSignature)
				{
					mEntities.push_back(i);
				}
			}
		}
		SceneView(Scene& scene)
		{
			static_assert(sizeof...(T) > 0, "Must have at least 1 component in scene view");

			ComponentTypeID ids[]{ GetComponentTypeID<T>()... };
			for (ComponentTypeID i : ids)
			{
				mSignature.set(i);
			}

			const std::vector<Signature>& sigs{ scene.GetSignatures() };
			for (Entity i : scene.GetIDHandler())
			{
				std::cout << i << "\n";
				if ((mSignature & (sigs.at(i))) == mSignature)
				{
					mEntities.push_back(i);
				}
			}
		}
	public:
		const std::vector<Entity>::const_iterator begin() const { return mEntities.begin(); }
		const std::vector<Entity>::const_iterator end() const { return mEntities.end(); }
	protected:
		std::vector<Entity> mEntities;
		Signature mSignature;
	};

}
