#pragma once

#include <axt/Core.h>
#include "Typedef.h"

namespace axt::ecs
{

	extern ComponentTypeID sComponentTypesRegistered;

	template<typename T>
	ComponentTypeID GetComponentTypeID()
	{
		static ComponentTypeID sTypeID{ sComponentTypesRegistered++ };
		return sTypeID;
	}

}
