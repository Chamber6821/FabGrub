cmake_minimum_required(VERSION 3.17)
project(FabGrub CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

if (NOT DEFINED FAST OR NOT ${FAST})
    find_program(CLANG_TIDY NAMES "clang-tidy" REQUIRED)
    set(CLANG_TIDY_COMMAND "${CLANG_TIDY};-p;${CMAKE_CURRENT_BINARY_DIR}")
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_COMMAND})
    execute_process(COMMAND ${CLANG_TIDY} "--version")
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # In LLVM 16 std::views::join available only with -fexperimental-library
    add_compile_options(-fexperimental-library)

    if (NOT WIN32)
        # GCC stdlib throws errors for some C++20 features
        add_compile_options(-stdlib=libc++)
    endif()
endif ()

include(packages.cmake)

add_subdirectory(cmd)
add_subdirectory(src)
add_subdirectory(test-utils)
add_subdirectory(tests)
add_subdirectory(it)

target_link_libraries(test-utils PRIVATE src)
target_link_libraries(tests PRIVATE test-utils)
target_link_libraries(tests PRIVATE src)
target_link_libraries(it PRIVATE test-utils)
target_link_libraries(it PRIVATE src)

get_property(apps DIRECTORY "cmd" PROPERTY BUILDSYSTEM_TARGETS)
foreach (app ${apps})
    target_link_libraries(${app} PRIVATE src)
endforeach ()
