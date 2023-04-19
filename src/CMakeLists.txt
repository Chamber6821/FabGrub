project(sources)
set(TARGET ${PROJECT_NAME})

file(GLOB_RECURSE SOURCES "*.cpp")
message(DEBUG "Sources: " ${SOURCES})

if (NOT "${SOURCES}" STREQUAL "")
    add_library(sources ${SOURCES})
    target_include_directories(${TARGET} PUBLIC .)
else ()
    add_library(sources INTERFACE)
endif ()
