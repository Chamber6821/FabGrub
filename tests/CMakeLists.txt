cmake_minimum_required(VERSION 3.17)
project(tests CXX)

set(CMAKE_CXX_STANDARD 20)
set(TARGET ${PROJECT_NAME})

include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.9
)

FetchContent_Declare(
        FakeIt
        GIT_REPOSITORY https://github.com/eranpeer/FakeIt.git
        GIT_TAG 2.3.2
)

file(GLOB_RECURSE SOURCES "*.cpp")
message(DEBUG "Test sources: " ${SOURCES})
add_executable(${TARGET} ${SOURCES})
target_include_directories(${TARGET} PUBLIC .)

FetchContent_MakeAvailable(doctest)
FetchContent_GetProperties(doctest)
target_link_libraries(${TARGET} doctest_with_main)
message(DEBUG "Include: " ${doctest_SOURCE_DIR}/doctest)
target_include_directories(${TARGET} PUBLIC ${doctest_SOURCE_DIR}/doctest)

FetchContent_MakeAvailable(FakeIt)
message(DEBUG "Include: " ${FETCHCONTENT_BASE_DIR}/fakeit-src/single_header/doctest)
target_include_directories(${TARGET} PUBLIC ${FETCHCONTENT_BASE_DIR}/fakeit-src/single_header/doctest)

include(${FETCHCONTENT_BASE_DIR}/doctest-src/scripts/cmake/doctest.cmake)
doctest_discover_tests(${TARGET})
