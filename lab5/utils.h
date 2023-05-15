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
    glm::vec3 getCenter(std::vector<glm::vec3> vertices);
    glm::mat4 getRotationMatrix(glm::vec3 G, glm::vec3 O, glm::vec3 viewUp);
    glm::mat4 getTranslationMatrix(glm::vec3 point);
    std::vector<glm::vec3> scaleToOne(std::vector<glm::vec3> vertices);
    glm::mat4 getZMirrorMatrix();
    std::vector<glm::vec3> transformVertices(std::vector<glm::vec3> vertices, glm::mat4 transformationMatrix);
    std::vector<glm::vec3> projectVertices(std::vector<glm::vec3> vertices, glm::mat4 projectionMatrix);
    glm::vec3 transformPoint(glm::vec3 point, glm::mat4 transformationMatrix);
    glm::mat4 getProjectionMatrix(glm::vec3  O, glm::vec3 G);
    void printMatrix(glm::mat4 rotationMatrix, std::string name);
    glm::mat4 getViewMatrix(glm::vec3 O, glm::vec3 G, glm::vec3 viewUp);
    void print_vertices(std::vector<glm::vec3> vertices);

}