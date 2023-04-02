cmake_minimum_required(VERSION 3.10)
project(lab2)

set(CMAKE_CXX_STANDARD 11)

find_package(glfw3 REQUIRED)

add_executable(lab2 main.cpp)

target_link_libraries(lab2 GL GLU GLEW glut glfw)
set(CMAKE_BUILD_TYPE Debug)