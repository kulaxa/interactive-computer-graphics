//
// Created by mario on 4/2/23.
//

#include <fstream>
#include <glm/glm.hpp>
#include <vector>

#ifndef LAB4_UTILS_H
#define LAB4_UTILS_H

;

#endif //LAB4_UTILS_H

namespace Utils {
    struct minMax {
        float XMin;
        float XMax;
        float YMin;
        float YMax;
        float ZMin;
        float ZMax;
    };


    void readVerticesAndFaces(std::string filename, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &faces);
    glm::vec3 readVec3FromLine(std::string line);
    glm::vec3 readControlPoint(std::string line);
    minMax getMinMax(std::vector<glm::vec3> vertices);
    std::vector<glm::vec3> scaleToOne(std::vector<glm::vec3> vertices);
    glm::vec3 getCenter(std::vector<glm::vec3> vertices);
    std::vector<glm::vec3> readControlPoints(std::string filename);
    glm::vec3 calculateBazierPoint(std::vector<glm::vec3> control_points, float t);
    std::vector<glm::vec3> calculateBazierCurve(std::vector<glm::vec3> control_points, int n);
    std::vector<glm::vec3> removeBackFaces(std::vector<glm::vec3> vertices, std::vector<glm::vec3> faces, glm::vec3 O);
    int factorial(int n);
    float calc_b(int i, int n, float t);

}