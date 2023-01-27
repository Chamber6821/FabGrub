cmake_minimum_required(VERSION 3.17)
project(mod_management CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(sources ${SOURCES})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} sources)
target_include_directories(${PROJECT_NAME} PUBLIC src)

include(cmake_tests.cmake)
add_tests(tests sources src)
