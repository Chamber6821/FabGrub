project(src)
set(TARGET ${PROJECT_NAME})

file(GLOB_RECURSE HEADERS "*.h") # for IDE
file(GLOB_RECURSE SOURCES "*.cpp")
message(DEBUG "Sources: " ${SOURCES})

add_library(${TARGET} ${HEADERS} ${SOURCES})

target_include_directories(${TARGET} PUBLIC .)

FetchContent_MakeAvailable(fmt Reactions pubgrub cpr)
target_link_libraries(${TARGET}
        PUBLIC fmt::fmt
        PUBLIC Reactions::Reactions
        PUBLIC pubgrub::pubgrub
        PUBLIC cpr::cpr)

# migrated to lib/nlohmann/json.hpp
#FetchContent_MakeAvailable(nlohmann_json)
#target_link_libraries(${TARGET} PUBLIC nlohmann_json::nlohmann_json)
