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


    glm::mat4 getRotationMatrix(glm::vec3 G, glm::vec3 O, glm::vec3 viewUp){
        glm::vec3 z = G - O;
        z = glm::normalize(z);
        glm::vec3 x = glm::cross(z, viewUp);
        x = glm::normalize(x);
        glm::vec3 y = glm::cross(z, x);
        y = glm::normalize(y);
       // glm::vec3 OGxOGxOGxviewUp = glm::cross(OG, OGxOGxviewUp);
        glm::mat4 rotationMatrix = glm::mat4(x.x, y.x, z.x, 0,
                                             x.y, y.y, z.y, 0,
                                             x.z, y.z, z.z, 0,
                                             0, 0, 0, 1);
        return rotationMatrix;
    }


    glm::mat4 getTranslationMatrix(glm::vec3 point){
        glm::mat4 translationMatrix = glm::mat4(1, 0, 0, 0,
                                                0, 1, 0, 0,
                                                0, 0, 1, 0,
                                                -point.x, -point.y, -point.z, 1);
        return translationMatrix;
    }


    glm::mat4 getZMirrorMatrix(){
        glm::mat4 zMirrorMatrix = glm::mat4(1, 0, 0, 0,
                                            0, 1, 0, 0,
                                            0, 0, -1, 0,
                                            0, 0, 0, 1);
        return zMirrorMatrix;
    }

    std::vector<glm::vec3> transformVertices(std::vector<glm::vec3> vertices, glm::mat4 transformationMatrix){
        std::vector<glm::vec3> new_vertices = std::vector<glm::vec3>();
        for (auto vertex: vertices){
            glm::vec4 new_vertex = transformationMatrix * glm::vec4(vertex.x, vertex.y, vertex.z, 1);
            new_vertices.push_back(glm::vec3(new_vertex.x, new_vertex.y, new_vertex.z));
        }
        return new_vertices;
    }

    std::vector<glm::vec3> projectVertices(std::vector<glm::vec3> vertices, glm::mat4 projectionMatrix){
        std::vector<glm::vec3> new_vertices = std::vector<glm::vec3>();
        for (auto vertex: vertices){
            glm::vec4 new_vertex =glm::vec4(vertex.x, vertex.y, vertex.z, 1) * projectionMatrix;
            new_vertices.push_back(glm::vec3(new_vertex.x, new_vertex.y, new_vertex.z));
        }
        return new_vertices;
    }

    glm::mat4 getViewMatrix(glm::vec3 O, glm::vec3 G, glm::vec3 viewUp){
        glm::mat4 rotationMatrix = getRotationMatrix(G, O, viewUp);
        glm::mat4 translationMatrix = getTranslationMatrix(-O);
        glm::mat4 z_mirrorMatrix = getZMirrorMatrix();
        glm::mat4 viewMatrix = rotationMatrix * translationMatrix * z_mirrorMatrix;
        return viewMatrix;
    }

    glm::mat4 getProjectionMatrix(glm::vec3  O, glm::vec3 G){
        float h = glm::length(G - O);
        glm::mat4 projectionMatrix = glm::mat4(1, 0, 0, 0,
                                               0, 1, 0, 0,
                                               0, 0, 0, 1/h,
                                               0, 0, 0, 0);
        return projectionMatrix;
    }

    glm::vec3 transformPoint(glm::vec3 point, glm::mat4 transformationMatrix){
        glm::vec4 new_point = transformationMatrix * glm::vec4(point.x, point.y, point.z, 1);
        return glm::vec3(new_point.x, new_point.y, new_point.z);
    }

}




