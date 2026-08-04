cmake_minimum_required(VERSION 3.24)
project(SDL2-PortingBase LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(warnings INTERFACE)
if(MSVC)
    target_compile_options(warnings INTERFACE /W4)
else()
    target_compile_options(warnings INTERFACE -Wall -Wextra -Wpedantic)
endif()

add_subdirectory(src/core)
add_subdirectory(src/renderer)
add_subdirectory(src/platform)
add_subdirectory(src/app)

enable_testing()
add_subdirectory(tests)