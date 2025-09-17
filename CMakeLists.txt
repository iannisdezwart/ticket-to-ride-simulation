cmake_minimum_required(VERSION 3.25)
project(SimulateProject LANGUAGES CXX)

# Compiler settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Werror)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# Find all source files
file(GLOB_RECURSE SRCFILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE TESTFILES "${CMAKE_SOURCE_DIR}/test/*.cpp")

# Find OR-Tools
find_package(ortools REQUIRED)

# Build library from source files
add_library(ttrs_lib STATIC ${SRCFILES})
target_link_libraries(ttrs_lib SDL2 SDL2_image SDL2_ttf SDL2_gfx ortools::ortools pthread)

# Build main executable
add_executable(simulate ${CMAKE_SOURCE_DIR}/simulate.cpp)
target_link_libraries(simulate ttrs_lib)

# Build test executable
add_executable(tests ${TESTFILES})
target_link_libraries(tests ttrs_lib gtest gtest_main)
