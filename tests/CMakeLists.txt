include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(core_test
    FrameTimerTest.cpp
    FrameStatsTest.cpp
)

target_link_libraries(core_test
    PRIVATE
        core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(core_test)