#pragma once

#include <axt/Core.h>

#include <axt/world/GameWorld.h>

#include <yaml-cpp/yaml.h>

/*
I'm having a hard time getting reflectance.
Gonna have to have specific serialize functions each time
*/

namespace axt::serial
{

	bool Pack(const std::string& filepath, Ref<necs::Scene>& scene);
	bool Unpack(const std::string& filepath);
	
}

//bool Pack(std::string& filepath, GameWorld& world)
	//{
	//	// Not until GameWorld is ready
	//	assert(false);
	//	return true;
	//}

	//bool Unpack(std::string& filepath)
	//{

	//	return false;
	//}
