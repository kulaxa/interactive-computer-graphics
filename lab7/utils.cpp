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


    std::vector<glm::vec3> scaleToOne(std::vector<glm::vec3> vertices) {
        minMax minMax = getMinMax(vertices);
        float max = minMax.XMax - minMax.XMin;
        if (minMax.YMax - minMax.YMin > max) {
            max = minMax.YMax - minMax.YMin;
        }
        if (minMax.ZMax - minMax.ZMin > max) {
            max = minMax.ZMax - minMax.ZMin;
        }
        for (auto &v: vertices) {
            v.x = v.x / max;
            v.y = v.y / max;
            v.z = v.z / max;
        }

        // move to center
        glm::vec3 center = getCenter(vertices);
        for (auto &v: vertices) {
            v.x = v.x - center.x;
            v.y = v.y - center.y;
            v.z = v.z - center.z;
        }
        return vertices;
    }

    glm::vec3 readControlPoint(std::string line) {
        std::stringstream ss(line);
        std::string dummy;
        glm::vec3 vec;
        ss >> vec.x >> vec.y >> vec.z;
        return vec;
    }

    minMax getMinMax(std::vector<glm::vec3> vertices) {
        minMax minMax;
        minMax.XMin = vertices[0].x;
        minMax.XMax = vertices[0].x;
        minMax.YMin = vertices[0].y;
        minMax.YMax = vertices[0].y;
        minMax.ZMin = vertices[0].z;
        minMax.ZMax = vertices[0].z;
        for (auto &v: vertices) {
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

    glm::vec3 getCenter(std::vector<glm::vec3> vertices) {
        glm::vec3 center;
        minMax minMax = getMinMax(vertices);
        center.x = (minMax.XMax + minMax.XMin) / 2;
        center.y = (minMax.YMax + minMax.YMin) / 2;
        center.z = (minMax.ZMax + minMax.ZMin) / 2;
        return center;
    }


    void readVerticesAndFaces(std::string filename, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &faces) {
        std::ifstream file(filename);

        int vertexCount = 0;
        int faceCount = 0;

        if (file.is_open()) {
            std::string line;
            // Count vertices and faces
            while (getline(file, line)) {
                if (line[0] == 'v' && line[1] == ' ') {
                    vertexCount++;
                } else if (line[0] == 'f' && line[1] == ' ') {
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
                if (line[0] == 'v' && line[1] == ' ') {
                    vertices.push_back(Utils::readVec3FromLine(line));
                } else if (line[0] == 'f' && line[1] == ' ') {
                    faces.push_back(Utils::readVec3FromLine(line));
                }
            }
            file.close();
        } else {
            std::cout << "Unable to open file" << std::endl;
        }
    }


    std::vector<glm::vec3> removeBackFaces(std::vector<glm::vec3> vertices, std::vector<glm::vec3> faces, glm::vec3 O) {
        std::vector<glm::vec3> new_faces = std::vector<glm::vec3>();
        for (auto face: faces) {
            glm::vec3 v1 = vertices[face.x - 1];
            glm::vec3 v2 = vertices[face.y - 1];
            glm::vec3 v3 = vertices[face.z - 1];
            // center of triangle
            glm::vec3 center = (v1 + v2 + v3) / 3.0f;
            glm::vec3 v1v2 = v2 - v1;
            glm::vec3 v1v3 = v3 - v1;
            glm::vec3 normal = glm::cross(v1v2, v1v3);
            glm::vec3 OG = center - O;
            float dot = glm::dot(normal, OG);
            if (dot > 0) {
                new_faces.push_back(face);
            }
        }
        return new_faces;
    }

    void
    calculateNormals(std::vector<glm::vec3> vertices, std::vector<glm::vec3> faces, std::vector<glm::vec3> &normals) {
        std::vector<int> numOfNeighbours = std::vector<int>(vertices.size(), 0);
        for (int i = 0; i < faces.size(); i++) {
            glm::vec3 v1 = vertices[faces[i].x - 1];
            glm::vec3 v2 = vertices[faces[i].y - 1];
            glm::vec3 v3 = vertices[faces[i].z - 1];
            glm::vec3 v1v2 = v2 - v1;
            glm::vec3 v1v3 = v3 - v1;
            glm::vec3 normal = glm::cross(v1v2, v1v3);
            normals[faces[i].x - 1] += normal;
            normals[faces[i].y - 1] += normal;
            normals[faces[i].z - 1] += normal;
            numOfNeighbours[faces[i].x - 1] += 1;
            numOfNeighbours[faces[i].y - 1] += 1;
            numOfNeighbours[faces[i].z - 1] += 1;
        }

        for (int i = 0; i < normals.size(); i++) {
            normals[i] = glm::normalize(normals[i] / (float) numOfNeighbours[i]);
        }
    }

    float
    constantLighting(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 L, glm::vec3 V, float Ii, float Ia, float Ka,
                     float kd, float ks, float n) {

        glm::vec3 N = glm::normalize(glm::cross(v2 - v1, v3 - v1));
        glm::vec3 R = glm::normalize(glm::reflect(-L, N));
        float k = 0.0001f;
        float r = glm::length(v1 - V);
        float ambient = Ia * Ka;
        float diffuse = kd * std::max(glm::dot(N, L), 0.0f);
        float reflective = ks * std::pow(std::max(glm::dot(R, V), 0.0f), n);
        float intensity = Ii / (r + k);

        return (ambient + intensity * (diffuse + reflective)) / 255.0f;

    }

    glm::vec3 gouraudoLighting(uint32_t v1_index, uint32_t v2_index, uint32_t v3_index, std::vector<glm::vec3> vertices,
                               std::vector<glm::vec3> normals, glm::vec3 L, glm::vec3 V, float Ii, float Ia, float Ka,
                               float kd, float ks, float n) {
        glm::vec3 norms[] = {normals[v1_index], normals[v2_index], normals[v3_index]};
        glm::vec3 v1 = vertices[v1_index];
        glm::vec3 result = glm::vec3(0.0f);
        for (int i = 0; i < 3; i++) {
            glm::vec3 R = glm::normalize(glm::reflect(-L, norms[i]));
            float k = 0.0001f;
            float r = glm::length(v1 - V);
            float ambient = Ia * Ka;
            float diffuse = kd * std::max(glm::dot(norms[i], L), 0.0f);
            float specular = ks * std::pow(std::max(glm::dot(R, V), 0.0f), n);
            float intensity = Ii / (r + k);


            result[i] = (ambient + intensity * (diffuse + specular)) / 255.0f;
        }
        return result;
    }

}




