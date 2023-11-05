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

    minMax getMinMax(std::vector<glm::vec3> vertices);

    std::vector<glm::vec3> scaleToOne(std::vector<glm::vec3> vertices);

    glm::vec3 getCenter(std::vector<glm::vec3> vertices);

    std::vector<glm::vec3> removeBackFaces(std::vector<glm::vec3> vertices, std::vector<glm::vec3> faces, glm::vec3 O);

    void
    calculateNormals(std::vector<glm::vec3> vertices, std::vector<glm::vec3> faces, std::vector<glm::vec3> &normals);

    float
    constantLighting(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 L, glm::vec3 V, float Ii, float Ia, float Ka,
                     float kd, float ks, float n);

    glm::vec3 gouraudoLighting(uint32_t v1_index, uint32_t v2_index, uint32_t v3_index, std::vector<glm::vec3> vertices,
                               std::vector<glm::vec3> normals, glm::vec3 L, glm::vec3 V, float Ii, float Ia, float Ka,
                               float kd, float ks, float n);
}