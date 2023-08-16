project(src)
set(TARGET ${PROJECT_NAME})

file(GLOB_RECURSE HEADERS "*.h") # for IDE
file(GLOB_RECURSE SOURCES "*.cpp")
message(DEBUG "Sources: " ${SOURCES})

add_library(${TARGET} ${HEADERS} ${SOURCES})

target_include_directories(${TARGET} PUBLIC .)

FetchContent_MakeAvailable(fmt)
target_link_libraries(${TARGET} PUBLIC fmt::fmt)

FetchContent_MakeAvailable(Reactions)
target_link_libraries(${TARGET} PUBLIC Reactions::Reactions)

FetchContent_MakeAvailable(pubgrub)
target_link_libraries(${TARGET} PUBLIC pubgrub::pubgrub)

FetchContent_MakeAvailable(cpr)
target_link_libraries(${TARGET} PUBLIC cpr::cpr)

FetchContent_MakeAvailable(nlohmann_json)
target_link_libraries(${TARGET} PUBLIC nlohmann_json::nlohmann_json)
