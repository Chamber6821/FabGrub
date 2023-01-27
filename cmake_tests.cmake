include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

function(add_tests TARGET SOURCE_LIBRARY SOURCE_FOLDER)
    file(GLOB_RECURSE TESTS "tests/*.cpp")
    add_executable(${TARGET} ${TESTS})

    target_link_libraries(${TARGET} ${SOURCE_LIBRARY})
    target_include_directories(${TARGET} PUBLIC ${SOURCE_FOLDER})


    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    target_link_libraries(${TARGET} GTest::gtest_main)
    target_link_libraries(${TARGET} GTest::gmock_main)

    include(GoogleTest)
    gtest_discover_tests(${TARGET})
endfunction()
