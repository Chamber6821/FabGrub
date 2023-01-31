cmake_minimum_required(VERSION 3.17)
project(sources CXX)

set(CMAKE_CXX_STANDARD 20)
set(TARGET ${PROJECT_NAME})

file(GLOB_RECURSE SOURCES "*.cpp")
message(DEBUG "Sources: " ${SOURCES})

if (NOT "${SOURCES}" STREQUAL "")
    add_library(sources ${SOURCES})
else ()
    add_library(sources INTERFACE)
endif ()

target_include_directories(${TARGET} INTERFACE .)
