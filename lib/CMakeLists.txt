project(lib)
set(TARGET ${PROJECT_NAME})

add_library(${TARGET} INTERFACE)
target_include_directories(${TARGET} INTERFACE .)
