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

FetchContent_MakeAvailable(SDL2)

add_library(platform STATIC
    Platform.cpp
    Entry.cpp
)

target_link_libraries(platform
    PUBLIC
        core
    PRIVATE
        SDL2::SDL2-static
        warnings
)