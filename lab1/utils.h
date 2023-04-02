#pragma once
#include <glm/glm.hpp>
namespace Utils {
	class Utils {
	public:
		static glm::vec3 solveLinearEqusion(glm::mat3 leftSide, glm::vec3 rightSide);
	};
}