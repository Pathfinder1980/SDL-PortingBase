cmake_minimum_required(VERSION 3.24)
project(SDL2-PortingBase LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.32.10
  GIT_SHALLOW TRUE
)
set(SDL_SHARED  OFF CACHE BOOL "" FORCE)
set(SDL_STATIC  ON  CACHE BOOL "" FORCE)
set(SDL_TEST    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL2 googletest)

find_package(OpenGL REQUIRED)

add_subdirectory(src/core) 

add_executable(SDL2-PortingBase)
target_sources(SDL2-PortingBase
    PRIVATE
        main.cpp
)

target_link_libraries(SDL2-PortingBase 
    PRIVATE
        core
        SDL2::SDL2-static
        OpenGL::GL)

enable_testing()
add_subdirectory(tests)