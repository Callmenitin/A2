cmake_minimum_required(VERSION 3.0)

project(a1)
set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

add_library(viewer src/hw.cpp src/viewer.cpp)
target_link_libraries(viewer GLEW::GLEW glm::glm OpenGL::GL SDL2::SDL2)

add_executable(sphere src/sphere.cpp)
target_link_libraries(sphere viewer)

add_executable(remesh src/remesh.cpp)
target_link_libraries(remesh viewer)



