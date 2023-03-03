#include <pch.h>

#include "OpenShader.h"

#include <fstream>

namespace axt {

	// Returns a text file as a string
	std::string OpenShader(const std::string filepath) {
		AXT_PROFILE_FUNCTION();

		std::string output;
		std::ifstream file;

		// TODO : SHOULD GET RID OF EXCEPTIONS
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