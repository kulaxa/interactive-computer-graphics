#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "utils.h"

int main() {
	std::cout << "Radni zadatak 1" << std::endl;

	glm::vec3 v1 = glm::vec3(2, 3, -4) + glm::vec3(1, 4, -1);
	std::cout << "v1 = " << glm::to_string(v1) << std::endl;

	float s = glm::dot(v1, glm::vec3(-1, 4, -1));
	std::cout << "s = " << s << std::endl;

	glm::vec3 v2 = glm::cross(v1, glm::vec3(2, 2, 4));
	std::cout << "v2 = " << glm::to_string(v2) << std::endl;

	glm::vec3 v3 = glm::normalize(v2);
	std::cout << "v3 = " << glm::to_string(v3) << std::endl;

	glm::vec3 v4 = -v2;
	std::cout << "v4 = " << glm::to_string(v4) << std::endl;

	glm::mat3 matrix1 = glm::mat3(1, 2, 3,
		2, 1, 3,
		4, 5, 1);
	glm::mat3 matrix2 = glm::mat3(-1, 2, -3,
		5, -2, 7,
		-4, -1, 3);

	glm::mat3 m1 = matrix1 + matrix2;
	std::cout << "m1 = " << glm::to_string(m1) << std::endl;

	glm::mat3 m2 = matrix1 * glm::transpose(matrix2);
	std::cout << "m2 = " << glm::to_string(m2) << std::endl;

	glm::mat3 m3 = matrix1 * glm::inverse(matrix2);
	std::cout << "m3 = " << glm::to_string(m3) << std::endl;


	std::cout << std::endl<<  "Radni zadatak 2" << std::endl;

	glm::mat3 leftSide = glm::zero<glm::mat3>();
	glm::vec3 rightSide = glm::zero<glm::vec3>();

	for (int i = 0; i < leftSide.length() ; i++) {
		for (int j = 0; j < leftSide.length() + 1; j++) {
			if (j == 3) {
				std::cin >> rightSide[i];
			}
			else {
				std::cin >> leftSide[j][i];
			}
		}
	}
	std::cout << "Result: " << glm::to_string(Utils::Utils::solveLinearEqusion(leftSide, rightSide)) << std::endl;


	std::cout << std::endl << "Radni zadatak 3" << std::endl;

	std::cout << "Enter (x, y, z) for point A: " << std::endl;
	glm::vec3 pointA = glm::zero<glm::vec3>();
	std::cin >> pointA.x >> pointA.y >> pointA.z;

	std::cout << "Enter (x, y, z) for point B: " << std::endl;
	glm::vec3 pointB = glm::zero<glm::vec3>();
	std::cin >> pointB.x >> pointB.y >> pointB.z;

	std::cout << "Enter (x, y, z) for point C: " << std::endl;
	glm::vec3 pointC = glm::zero<glm::vec3>();
	std::cin >> pointC.x >> pointC.y >> pointC.z;

	std::cout << "Enter (x, y, z) for point T: " << std::endl;
	glm::vec3 pointT = glm::zero<glm::vec3>();
	std::cin >> pointT.x >> pointT.y >> pointT.z;

	glm::mat3 matrix = glm::mat3(pointA.x, pointB.x, pointC.x,
								pointA.y, pointB.y, pointC.y,
								pointA.z, pointB.z, pointC.z);
	glm::vec3 result = Utils::Utils::solveLinearEqusion(matrix, pointT);
	std::cout << "Baricentric coordinates for given triangle and point T are: " << glm::to_string(result) << std::endl;

}