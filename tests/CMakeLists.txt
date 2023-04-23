project(tests)
set(TARGET ${PROJECT_NAME})

include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.9
)

file(GLOB_RECURSE TESTS "*Test.cpp")
add_executable(${TARGET} ${TESTS})

FetchContent_MakeAvailable(doctest)
FetchContent_GetProperties(doctest)
target_link_libraries(${TARGET} doctest_with_main)
target_include_directories(${TARGET} PUBLIC ${doctest_SOURCE_DIR}/doctest)

include(${FETCHCONTENT_BASE_DIR}/doctest-src/scripts/cmake/doctest.cmake)
doctest_discover_tests(${TARGET})
