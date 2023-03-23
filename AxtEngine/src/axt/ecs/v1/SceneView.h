#pragma once

#include "axt/Core.h"

#include "Scene.h"
#include "ComponentPack.h"

namespace axt::ecs
{

	template<typename... T>
	class AXT_API SceneView
	{
	public:
		SceneView(Scene& scene) :
			mScene{ NewRef<Scene>(scene) }
		{
			ComponentID ids[]{ GetComponentTypeID<T>()... };
			for (int32_t i{ 0 }; i < sizeof...(T); i++) // sizeof...(T) is the amount of template arguments passed
			{
				mSignature.set(ids[i]);
			}
			AXT_TRACE(mSignature.to_string());
		}
	public:
		class iterator
		{
			iterator(Scene& scene, SceneView& view) : 
				mScene{ scene }, mView{ view }
			{};
			EntityID operator*() const
			{
				return mScene.mEntityInfo[mIndex].ID;
			}
			bool operator==(const iterator& other) const
			{
				return mIndex == other.mIndex;
			}
			bool operator!=(const iterator& other) const
			{
				return mIndex != other.mIndex;
			}
			iterator& operator++() const
			{

			}
		protected:
			Scene& mScene;
			SceneView& mView;
			EntityID mIndex{ 0 };
		};
		const iterator being() const;
		const iterator end() const;
	protected:
		Signature mSignature;
		Ref<Scene> mScene;
	};

}
