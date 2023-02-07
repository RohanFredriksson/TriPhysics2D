cmake_minimum_required(VERSION 3.14)

set(project_name trip2d)
project(${project_name})

#add_subdirectory(dependencies/glm/)

if (WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -g")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    set(CMAKE_VERBOSE_MAKEFILE ON)
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -g")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -fsanitize=address")
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

include_directories(include)
include_directories(dependencies/glm)

file(GLOB_RECURSE source_files src/*.cpp)
add_library(${project_name} ${source_files})

#target_include_directories(${project_name} PUBLIC glm)
#target_link_libraries(${project_name} PUBLIC glm)