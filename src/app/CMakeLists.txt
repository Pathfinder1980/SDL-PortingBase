file(GLOB_RECURSE ASSET_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/assets/*)

set(ASSET_STAMP ${CMAKE_CURRENT_BINARY_DIR}/assets.stamp)
add_custom_command(OUTPUT ${ASSET_STAMP}
    COMMAND ${CMAKE_COMMAND} -E rm -rf $<TARGET_FILE_DIR:SDL2-PortingBase>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SDL2-PortingBase>/assets
    COMMAND ${CMAKE_COMMAND} -E touch ${ASSET_STAMP}
    DEPENDS ${ASSET_FILES}
    COMMENT "Syncing assets next to the executable"
    VERBATIM)

add_custom_target(assets DEPENDS ${ASSET_STAMP})

add_executable(SDL2-PortingBase
     AppMain.cpp
)

add_dependencies(SDL2-PortingBase 
    assets
)

target_link_libraries(SDL2-PortingBase
    PRIVATE
        renderer
        platform
        core
        warnings
)