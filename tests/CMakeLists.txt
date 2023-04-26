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
target_link_libraries(${TARGET} PRIVATE doctest_with_main)
set_target_properties(doctest_with_main PROPERTIES CXX_CLANG_TIDY "")

include(${FETCHCONTENT_BASE_DIR}/doctest-src/scripts/cmake/doctest.cmake)
doctest_discover_tests(${TARGET})
