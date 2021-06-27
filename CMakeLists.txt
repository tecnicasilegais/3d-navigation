cmake_minimum_required(VERSION 3.19)
project(3D_Nav)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB IMAGE "image_libs/*")
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "headers/*.h")


add_executable(3D_Nav ${SOURCES} ${IMAGE} ${HEADERS} main.cpp headers/shader.h headers/shader.h)
target_link_libraries(3D_Nav -lglut -lGLU -lGL -lSOIL -lGLEW)