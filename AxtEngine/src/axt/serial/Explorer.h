#pragma once

#include <axt/Core.h>

#include <string>

namespace axt::explorer
{

	std::string OpenFile(const char* filter);
	std::string SaveFile(const char* filter);

	//HRESULT CreateOpenFileWindow();

}
