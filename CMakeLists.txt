cmake_minimum_required(VERSION 3.17)
project(FabGrub CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-Ulinux -Uunix)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # In LLVM 16 std::views::join available only with -fexperimental-library
    add_compile_options(-fexperimental-library)
endif ()

include(packages.cmake)

add_subdirectory(cmd)
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test-utils)
add_subdirectory(tests)

target_link_libraries(src PUBLIC lib)
target_link_libraries(test-utils PRIVATE src)
target_link_libraries(tests PRIVATE src PRIVATE test-utils)

get_property(apps DIRECTORY "cmd" PROPERTY BUILDSYSTEM_TARGETS)
foreach (app ${apps})
    target_link_libraries(${app} PRIVATE src)
endforeach ()

if ("${LINT}")
    find_program(CLANG_TIDY NAMES "clang-tidy" REQUIRED)
    set(CLANG_TIDY_COMMAND "${CLANG_TIDY};-p;${CMAKE_CURRENT_BINARY_DIR};--use-color")
    set_target_properties(src test-utils tests ${apps} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif ()
