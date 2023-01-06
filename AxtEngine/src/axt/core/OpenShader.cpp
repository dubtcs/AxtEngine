#include <pch.h>

#include "OpenShader.h"

#include <fstream>

namespace axt {

	std::string OpenShader(const std::string filepath) {
		std::string output;
		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			file.open(filepath);
			std::stringstream str;
			str << file.rdbuf();
			file.close();
			output = str.str();
		}
		catch(std::ifstream::failure& er) {
			AXT_CORE_ERROR("File open failure: {0}", er.what());
		}
		return output;
	}

}