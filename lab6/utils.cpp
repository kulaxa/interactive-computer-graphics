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

    glm::vec3 readControlPoint(std::string line){
        std::stringstream ss(line);
        std::string dummy;
        glm::vec3 vec;
        ss  >> vec.x >> vec.y >> vec.z;
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

    std::vector<glm::vec3> readControlPoints(std::string filename){
        std::ifstream file(filename);

        int points_count = 0;
        std::vector<glm::vec3> control_points = std::vector<glm::vec3>();


        if (file.is_open()) {
            std::string line;
            // Count vertices and faces
            while (getline(file, line)) {
                glm::vec3 control_point = Utils::readControlPoint(line);
                control_points.push_back(control_point);
                points_count++;
            }
            std::cout << "Control point count: " << points_count << std::endl;
            file.close();
        } else {
            std::cout << "Unable to open file" << std::endl;
        }
        return control_points;

    }    std::vector<glm::vec3> scaleToOne(std::vector<glm::vec3> vertices){
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

    glm::vec3 calculateBazierPoint(std::vector<glm::vec3> control_points, float t){
        glm::vec3 bazier_point = glm::vec3(0,0,0);
        for(int i= 0; i< control_points.size(); i++){
            glm::vec3 point = control_points[i];
            float b = calc_b(i, control_points.size(), t);
            point.x = point.x * b;
            point.y = point.y * b;
            point.z = point.z * b;
            bazier_point += point;
        }
        return bazier_point;
    }

    std::vector<glm::vec3> calculateBazierCurve(std::vector<glm::vec3> control_points, int n) {
        std::vector<glm::vec3> bazier_curve = std::vector<glm::vec3>();
        for (int i = 0; i < n; i++) {
            float t = (float) i / (float) n;
            glm::vec3 bazier_point = calculateBazierPoint(control_points, t);
            bazier_curve.push_back(bazier_point);
        }
        return bazier_curve;

    }

    int factorial(int n){
        if (n == 0 || n == 1) {
            return 1;
        } else {
            return n * factorial(n - 1);
        }
    }

    float calc_b(int i, int n, float t){
        float n_fact =  factorial(n-1);
        float i_fact = factorial(i);
        float n_i_fact = factorial(n-1-i);
        float t_to_i = pow(t, i);
        float t_to_n_i = pow(1-t, n-1-i);
        return (n_fact/(i_fact*n_i_fact))*t_to_i*t_to_n_i;
    }

    std::vector<glm::vec3> removeBackFaces(std::vector<glm::vec3> vertices, std::vector<glm::vec3> faces, glm::vec3 O){
        std::vector<glm::vec3> new_faces = std::vector<glm::vec3>();
        for (auto face: faces){
            glm::vec3 v1 = vertices[face.x-1];
            glm::vec3 v2 = vertices[face.y-1];
            glm::vec3 v3 = vertices[face.z-1];
            // center of triangle
            glm::vec3 center = (v1 + v2 + v3) / 3.0f;
            glm::vec3 v1v2 = v2 - v1;
            glm::vec3 v1v3 = v3 - v1;
            glm::vec3 normal = glm::cross(v1v2, v1v3);
            glm::vec3 OG = center - O;
            float dot = glm::dot(normal, OG);
            if (dot > 0){
                new_faces.push_back(face);
            }
        }
        return new_faces;
    }

}




