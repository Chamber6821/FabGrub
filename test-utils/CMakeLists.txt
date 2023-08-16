project(test-utils)
set(TARGET ${PROJECT_NAME})

file(GLOB_RECURSE HEADERS "*.h") # for IDE
file(GLOB_RECURSE SOURCES "*.cpp")
message(DEBUG "Sources: " ${SOURCES})

add_library(${TARGET} ${HEADERS} ${SOURCES})

target_include_directories(${TARGET} PUBLIC .)
