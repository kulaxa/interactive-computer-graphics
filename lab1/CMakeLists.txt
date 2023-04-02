cmake_minimum_required(VERSION 3.10)
project(lab1)

set(CMAKE_CXX_STANDARD 11)

find_package(glfw3 REQUIRED)


add_executable(lab1 main.cpp utils.h utils.cpp)

target_link_libraries(lab1 GL GLU GLEW glut glfw)
set(CMAKE_BUILD_TYPE Debug)
