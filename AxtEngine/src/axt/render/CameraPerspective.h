#pragma once

#include <axt/Core.h>

#include <glm/glm.hpp>

namespace axt
{

	class AXT_API CameraPerspective {
	public:
		CameraPerspective(const glm::vec3& origin, const glm::vec3& direction);
		~CameraPerspective();
	public:
		void Translate(const glm::vec3& translation);
		void Rotate(const glm::vec3& rotation);
	protected:
		glm::vec3 position;
		glm::vec3 direction;
	};

}
