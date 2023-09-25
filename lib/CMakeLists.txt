project(lib)
set(TARGET ${PROJECT_NAME})

add_library(${TARGET} ctrl-c/ctrl-c.cpp)
target_include_directories(${TARGET} PUBLIC .)
