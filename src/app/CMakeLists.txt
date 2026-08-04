add_executable(SDL2-PortingBase
     AppMain.cpp
)

target_link_libraries(SDL2-PortingBase
    PRIVATE
        platform
        core
        warnings
)