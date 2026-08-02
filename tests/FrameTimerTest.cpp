#include "core/FrameTimer.h"

#include <gtest/gtest.h>

using namespace porting_base;

TEST(FrameTimer, PartialFrameProducesNoStep)
{
    constexpr double fixedTimeStep = 0.01;
    constexpr double maxFrameTime = 0.25;
    FrameTimer timer(fixedTimeStep, maxFrameTime);
    EXPECT_EQ(timer.Advance(0.005), 0);
    EXPECT_NEAR(timer.GetAlphaTime(), 0.5, 1e-9);

    EXPECT_EQ(timer.Advance(0.005), 1);
    EXPECT_NEAR(timer.GetAlphaTime(), 0.0, 1e-9);
}

TEST(FrameTimer, MultipleSteps)
{
    constexpr double fixedTimeStep = 0.01;
    constexpr double maxFrameTime = 0.25;
    FrameTimer timer(fixedTimeStep, maxFrameTime);
    EXPECT_EQ(timer.Advance(0.035), 3);
    EXPECT_NEAR(timer.GetAlphaTime(), 0.5, 1e-9);
}

TEST(FrameTimer, MaxFrameTime)
{
    constexpr double fixedTimeStep = 0.01;
    constexpr double maxFrameTime = 0.25;
    FrameTimer timer(fixedTimeStep, maxFrameTime);
    EXPECT_EQ(timer.Advance(10.0), 25);
    EXPECT_NEAR(timer.GetAlphaTime(), 0.0, 1e-9);
}

TEST(FrameTimer, NegativeFrameTime)
{
    constexpr double fixedTimeStep = 0.01;
    constexpr double maxFrameTime = 0.25;
    FrameTimer timer(fixedTimeStep, maxFrameTime);
    EXPECT_EQ(timer.Advance(-0.1), 0);
    EXPECT_NEAR(timer.GetAlphaTime(), 0.0, 1e-9);
}