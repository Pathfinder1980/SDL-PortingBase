cmake_minimum_required(VERSION 3.24)
project(SDL2-BasePortingFramework LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



include(FetchContent)

set(SDL_SHARED  OFF CACHE BOOL "" FORCE)
set(SDL_STATIC  ON  CACHE BOOL "" FORCE)
set(SDL_TEST    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.32.10
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL2)

add_executable(SDL2-BasePortingFramework)
target_sources(SDL2-BasePortingFramework
    PRIVATE
        main.cpp
)

target_link_libraries(SDL2-BasePortingFramework PRIVATE SDL2::SDL2-static)