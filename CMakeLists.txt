cmake_minimum_required(VERSION 3.17)
project(FabGrub CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(cmd)
add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(tests sources)
target_include_directories(tests PUBLIC src)

get_property(apps DIRECTORY "cmd" PROPERTY BUILDSYSTEM_TARGETS)
foreach (app ${apps})
    target_link_libraries(${app} sources)
    target_include_directories(${app} PUBLIC src)
endforeach ()
