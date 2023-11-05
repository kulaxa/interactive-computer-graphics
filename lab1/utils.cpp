#include "utils.h"
#include <stdexcept>

namespace Utils {

	glm::vec3 Utils::solveLinearEqusion(glm::mat3 leftSide, glm::vec3 rightSide)
	{
		float determinate = glm::determinant(leftSide);
		if (determinate == 0) {
			throw std::invalid_argument("Cannot solve given equastions because left side doesn't have inverse!");
		}
		glm::vec3 result = glm::inverse(leftSide) * rightSide;
		return result;
	}
}