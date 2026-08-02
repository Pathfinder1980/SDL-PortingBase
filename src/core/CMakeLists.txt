add_library(core STATIC
    FrameTimer.cpp
    FrameStats.cpp
)

target_include_directories(core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)