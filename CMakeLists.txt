cmake_minimum_required(VERSION 3.17)
project(mod_management CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})