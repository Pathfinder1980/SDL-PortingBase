add_library(core STATIC
    FrameTimer.cpp
    FrameStats.cpp
    InputState.cpp
)

target_include_directories(core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
    PRIVATE
        warnings
)