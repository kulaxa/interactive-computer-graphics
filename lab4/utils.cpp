//
// Created by mario on 4/2/23.
//
#include <sstream>
#include <iostream>
#include "utils.h"

namespace  Utils {
    glm::vec3 readVec3FromLine(std::string line) {
        std::stringstream ss(line);
        std::string dummy;
        glm::vec3 vec;
        ss >> dummy >> vec.x >> vec.y >> vec.z;
        return vec;
    }

    minMax getMinMax(std::vector<glm::vec3> vertices){
        minMax minMax;
        minMax.XMin = vertices[0].x;
        minMax.XMax = vertices[0].x;
        minMax.YMin = vertices[0].y;
        minMax.YMax = vertices[0].y;
        minMax.ZMin = vertices[0].z;
        minMax.ZMax = vertices[0].z;
        for (auto &v : vertices) {
            if (v.x < minMax.XMin) {
                minMax.XMin = v.x;
            }
            if (v.x > minMax.XMax) {
                minMax.XMax = v.x;
            }
            if (v.y < minMax.YMin) {
                minMax.YMin = v.y;
            }
            if (v.y > minMax.YMax) {
                minMax.YMax = v.y;
            }
            if (v.z < minMax.ZMin) {
                minMax.ZMin = v.z;
            }
            if (v.z > minMax.ZMax) {
                minMax.ZMax = v.z;
            }
        }
        return minMax;

    }

    glm::vec3 getCenter(std::vector<glm::vec3> vertices){
        glm::vec3 center;
        minMax minMax = getMinMax(vertices);
        center.x = (minMax.XMax + minMax.XMin) / 2;
        center.y = (minMax.YMax + minMax.YMin) / 2;
        center.z = (minMax.ZMax + minMax.ZMin) / 2;
        return center;
    }

    std::vector<glm::vec3> scaleToOne(std::vector<glm::vec3> vertices){
        minMax minMax = getMinMax(vertices);
        float max = minMax.XMax - minMax.XMin;
        if (minMax.YMax - minMax.YMin > max) {
            max = minMax.YMax - minMax.YMin;
        }
        if (minMax.ZMax - minMax.ZMin > max) {
            max = minMax.ZMax - minMax.ZMin;
        }
        for (auto &v : vertices) {
            v.x = v.x / max;
            v.y = v.y / max;
            v.z = v.z / max;
        }

        // move to center
        glm::vec3 center = getCenter(vertices);
        for (auto &v : vertices) {
            v.x = v.x - center.x;
            v.y = v.y - center.y;
            v.z = v.z - center.z;
        }
        return vertices;
    }

    planeCoefficients getPlaneCoefficients(glm::vec3 face, std::vector<glm::vec3>  vertices){
        planeCoefficients planeCoefficients;
        glm::vec3 p1 = vertices[face.x-1];
        glm::vec3 p2 = vertices[face.y-1];
        glm::vec3 p3 = vertices[face.z-1];

        planeCoefficients.A = (p2.y - p1.y) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.y - p1.y);
        planeCoefficients.B = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
        planeCoefficients.C = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
        planeCoefficients.D = -planeCoefficients.A * p1.x - planeCoefficients.B * p1.y - planeCoefficients.C * p1.z;

        return planeCoefficients;
    }

    void readVerticesAndFaces(std::string filename, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &faces){
        std::ifstream file(filename);

        int vertexCount = 0;
        int faceCount = 0;

        if (file.is_open()) {
            std::string line;
            // Count vertices and faces
            while (getline(file, line)) {
                if(line[0] == 'v' && line[1] == ' ') {
                    vertexCount++;
                } else if(line[0] == 'f' && line[1] == ' '){
                    faceCount++;
                }
            }
            std::cout << "Vertices: " << vertexCount << std::endl;
            std::cout << "Faces: " << faceCount << std::endl;
            std::cout << "Allocating memory" << std::endl;

            vertices.reserve(vertexCount);
            faces.reserve(faceCount);

            std::cout << "Reading data" << std::endl;
            file.clear();
            file.seekg(0, std::ios::beg);

            while (getline(file, line)) {
                if(line[0] == 'v' && line[1] == ' ') {
                    vertices.push_back(Utils::readVec3FromLine(line));
                } else if(line[0] == 'f' && line[1] == ' '){
                    faces.push_back(Utils::readVec3FromLine(line));
                }
            }
            file.close();
        } else {
            std::cout << "Unable to open file" << std::endl;
        }
    }

    std::vector<planeCoefficients> getAllPlanesCoefficients(std::vector<glm::vec3> faces, std::vector<glm::vec3> vertices){
        std::vector<planeCoefficients> planesCoefficients;
        for (auto &face : faces) {
            planesCoefficients.push_back(Utils::getPlaneCoefficients(face, vertices));
        }
        return planesCoefficients;
    }

    bool isPointInside(glm::vec3 point, std::vector<planeCoefficients> planesCoefficients, bool convex){
        for (auto plane : planesCoefficients) {
            if (convex) {
                if (plane.A * point.x + plane.B * point.y + plane.C * point.z + plane.D > 0) {
                    return false;
                }
            }
            else{
                return false;
            }
        }
        return true;
    }


}