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

    struct planeCoefficients {
        float A;
        float B;
        float C;
        float D;
    };

    void readVerticesAndFaces(std::string filename, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &faces);
    glm::vec3 readVec3FromLine(std::string line);
    minMax getMinMax(std::vector<glm::vec3> vertices);
    glm::vec3 getCenter(std::vector<glm::vec3> vertices);
    std::vector<glm::vec3> scaleToOne(std::vector<glm::vec3> vertices);
    planeCoefficients getPlaneCoefficients(glm::vec3 trianglePointsIndexes, std::vector<glm::vec3> vertices);
    std::vector<planeCoefficients> getAllPlanesCoefficients(std::vector<glm::vec3> faces, std::vector<glm::vec3> vertices);
    bool isPointInside(glm::vec3 point, std::vector<planeCoefficients> planesCoefficients, bool convex);

}