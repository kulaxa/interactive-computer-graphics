//
// Created by mario on 4/1/23.
//
#include <iostream>
#include <algorithm>

#include <glm/common.hpp>
#include "utils.h"

bool Utils::compare(glm::vec2 a, glm::vec2 b, glm::vec2 center) {
    float dx1 = a.x - center.x;
    float dy1 = a.y - center.y;
    float dx2 = b.x - center.x;
    float dy2 = b.y - center.y;

    if (atan2(dy1, dx1) < atan2(dy2, dx2))
        return true;
    else if (atan2(dy1, dx1) > atan2(dy2, dx2))
        return false;
    else
        return sqrt(dx1*dx1 + dy1*dy1) < sqrt(dx2*dx2 + dy2*dy2);
}

void Utils::sortClockWise(std::vector<glm::vec2> & points){
    double sum_x = 0, sum_y = 0;
    for (auto p : points) {
        sum_x += p.x;
        sum_y += p.y;
    }
    glm::vec2 center = {sum_x / points.size(), sum_y / points.size()};

    sort(points.begin(), points.end(), [center](glm::vec2 a, glm::vec2 b) {
        return Utils::compare(a, b, center);
    });
}


void Utils::sortLeftRightEdges(std::vector<glm::vec2> & orderedPoints, std::vector<Utils::Edge> & leftEdges, std::vector<Utils::Edge> & rightEdges){
    for (int i = 0; i < orderedPoints.size(); i++) {
        Utils::Edge edge;
        edge.start = orderedPoints[i];
        edge.end = orderedPoints[(i + 1) % orderedPoints.size()];
        if (edge.start.y < edge.end.y) {
            leftEdges.push_back(edge);
        } else {
            rightEdges.push_back(edge);
        }
    }
}

bool Utils::compareY(glm::vec2 a, glm::vec2 b){
    return a.y < b.y;
}

glm::vec2 Utils::calculateIntersection(Utils::Edge & edge, float y){
    float x = edge.start.x + (edge.end.x - edge.start.x) * (y - edge.start.y) / (edge.end.y - edge.start.y);
    return {x, y};
}

std::vector<Utils::LineCoefficients> Utils::calculateLineCoefficients(std::vector<glm::vec2> & orderedPoints){
    std::vector<LineCoefficients> coefficientsVector;
    for(int i = 0; i < orderedPoints.size(); i++){
        float x1 = orderedPoints[i].x;
        float y1 = orderedPoints[i].y;
        float x2 = orderedPoints[(i+1) % orderedPoints.size()].x;
        float y2 = orderedPoints[(i+1) % orderedPoints.size()].y;
        LineCoefficients coefficients;
        coefficients.a = y1 - y2;
        coefficients.b = - x1 + x2;
        coefficients.c = x1 * y2 - x2 * y1;
        coefficientsVector.push_back(coefficients);
    }
    return coefficientsVector;


}

bool Utils::checkPointInPolygon(glm::vec2 point, std::vector<LineCoefficients> & coefficients){
    for(auto p : coefficients){
        if(p.a * point.x + p.b * point.y + p.c < 0){
            return false;
        }
    }
    return true;
}


