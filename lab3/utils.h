//
// Created by mario on 4/1/23.
//

#include <glm/vec2.hpp>
#include <vector>

#ifndef LAB3_UTILS_H
#define LAB3_UTILS_H

#endif //LAB3_UTILS_H


namespace Utils{

    struct Edge {
        glm::vec2 start;
        glm::vec2 end;
    };

    struct LineCoefficients {
        float a;
        float b;
        float c;
    };


    bool compare(glm::vec2 a, glm::vec2 b, glm::vec2 center);
    bool compareY(glm::vec2 a, glm::vec2 b);
    void sortClockWise(std::vector<glm::vec2> & points);
    void sortLeftRightEdges(std::vector<glm::vec2> & orderedPoints, std::vector<Edge> & leftEdges, std::vector<Edge> & rightEdges);
    glm::vec2 calculateIntersection(Edge & edge, float y);
    std::vector<LineCoefficients> calculateLineCoefficients(std::vector<glm::vec2> & orderedPoints);
    bool checkPointInPolygon(glm::vec2 point, std::vector<LineCoefficients> & coefficients);
}