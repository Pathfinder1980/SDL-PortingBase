add_library(renderer STATIC
    Renderer.cpp
    GLApi.cpp
)

target_include_directories(renderer
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(renderer
    PUBLIC
        core
    PRIVATE
        warnings
)