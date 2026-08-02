#include "core/FrameStats.h"

#include <gtest/gtest.h>

using namespace porting_base;

TEST(FrameStats, DefaultConstructorInitializesToZero)
{
    FrameStats stats;
    EXPECT_DOUBLE_EQ(stats.GetAccumulatedFrameTimeInSeconds(), 0.0);
    EXPECT_DOUBLE_EQ(stats.GetAverageFPS(), 0.0);
    EXPECT_DOUBLE_EQ(stats.GetAverageFrameTimeInMilliseconds(), 0.0);
}

TEST(FrameStats, UpdateAccumulatesFrameTimeAndCount)
{
    FrameStats stats;
    stats.Update(0.25);
    stats.Update(0.25);
    EXPECT_NEAR(stats.GetAccumulatedFrameTimeInSeconds(), 0.5, 1e-9);
    EXPECT_NEAR(stats.GetAverageFPS(), 2 / 0.5, 1e-9);
    EXPECT_NEAR(stats.GetAverageFrameTimeInMilliseconds(), 250.0, 1e-9);
}

TEST(FrameStats, ResetClearsAccumulatedTimeAndCount)
{
    FrameStats stats;
    stats.Update(0.25);
    stats.Reset();
    EXPECT_DOUBLE_EQ(stats.GetAccumulatedFrameTimeInSeconds(), 0.0);
    EXPECT_DOUBLE_EQ(stats.GetAverageFPS(), 0.0);
    EXPECT_DOUBLE_EQ(stats.GetAverageFrameTimeInMilliseconds(), 0.0);
}