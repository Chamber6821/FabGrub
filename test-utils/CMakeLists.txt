project(test-utils)
set(TARGET ${PROJECT_NAME})

file(GLOB_RECURSE SOURCES "*.cpp")
message(DEBUG "Sources: " ${SOURCES})

if (NOT "${SOURCES}" STREQUAL "")
    add_library(${TARGET} ${SOURCES})
    target_include_directories(${TARGET} PUBLIC .)
else ()
    add_library(${TARGET} INTERFACE)
endif ()
