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