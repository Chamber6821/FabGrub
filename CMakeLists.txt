cmake_minimum_required(VERSION 3.17)
project(mod_management CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} sources)
target_include_directories(${PROJECT_NAME} PUBLIC src)

target_link_libraries(tests sources)
target_include_directories(tests PUBLIC src)
