#pragma once

#include <axt/Core.h>

#include "Scene.h"
#include "ComponentPack.h"

namespace axt::ecs
{

	// returns a list of EntityIDs that have the requested components attached
	template<typename... T>
	class AXT_API SceneView
	{
	public:
		SceneView(const Ref<Scene>&scene) : mScene{ scene }
		{
			static_assert(sizeof...(T) > 0, "Must have 1 or more components in scene view.");
			{
				ComponentTypeID ids[]{ GetComponentTypeID<T>()... };
				for (uint32_t i{ 0 }; i < sizeof...(T); i++)
				{
					mSignature.set(ids[i]);
				}
			}
			Ref<std::array<Scene::EntityInfo, gMaxEntities>> info{ mScene->GetEntityInfo() };
			for (EntityID i : mScene->GetIDManager())
			{
				if ((mSignature & (info->at(i).Mask)) == mSignature)
				{
					mEntities.push_back(info->at(i).ID);
				}
			}
			// maybe save the ids to each pack or system manager?
		}
	public:
		const std::vector<EntityID>::const_iterator begin() const { return mEntities.begin(); }
		const std::vector<EntityID>::const_iterator end() const { return mEntities.end(); }
	protected:
		Ref<Scene> mScene;
		Signature mSignature;
		std::vector<EntityID> mEntities;
	};

}
