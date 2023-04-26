project(src)
set(TARGET ${PROJECT_NAME})

file(GLOB_RECURSE SOURCES "*.cpp")
message(DEBUG "Sources: " ${SOURCES})

if (NOT "${SOURCES}" STREQUAL "")
    add_library(${TARGET} ${SOURCES})
else ()
    file(GENERATE OUTPUT ${TARGET}-stub.cpp CONTENT "")
    add_library(${TARGET} ${TARGET}-stub.cpp)
endif ()

target_include_directories(${TARGET} PUBLIC .)
