project(it)
set(TARGET ${PROJECT_NAME})

file(GLOB_RECURSE TESTS "*Test.cpp")
add_executable(${TARGET} ${TESTS})

FetchContent_MakeAvailable(doctest)
target_link_libraries(${TARGET} PRIVATE doctest_with_main)

include(${FETCHCONTENT_BASE_DIR}/doctest-src/scripts/cmake/doctest.cmake)
doctest_discover_tests(${TARGET})
