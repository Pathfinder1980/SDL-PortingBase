#include "core/InputState.h"

#include <gtest/gtest.h>

using namespace porting_base;

TEST(InputState, InputStateAxisNormalization)
{
    // Test normalization with deadzone
    EXPECT_FLOAT_EQ(NormalizeAxis(0, 0.1f), 0.0f);
    EXPECT_FLOAT_EQ(NormalizeAxis(32767, 0.1f), 1.0f);
    EXPECT_FLOAT_EQ(NormalizeAxis(-32768, 0.1f), -1.0f);
    EXPECT_FLOAT_EQ(NormalizeAxis(16384, 0.1f), (16384 / 32767.0f - 0.1f) / (1.0f - 0.1f));
    EXPECT_FLOAT_EQ(NormalizeAxis(-16384, 0.1f), (-16384 / 32767.0f + 0.1f) / (1.0f - 0.1f));
}

TEST(InputState, InputStateComputeEdges)
{
    InputState previous;
    InputState current;

    // Simulate pressing the Highlight action
    previous.actions[static_cast<size_t>(Action::Highlight)] = false;
    current.actions[static_cast<size_t>(Action::Highlight)] = true;

    InputEdges edges = ComputeInputEdges(previous, current);
    EXPECT_TRUE(edges.pressed[static_cast<size_t>(Action::Highlight)]);
    EXPECT_FALSE(edges.released[static_cast<size_t>(Action::Highlight)]);

    // Simulate releasing the Highlight action
    previous.actions[static_cast<size_t>(Action::Highlight)] = true;
    current.actions[static_cast<size_t>(Action::Highlight)] = false;

    edges = ComputeInputEdges(previous, current);
    EXPECT_FALSE(edges.pressed[static_cast<size_t>(Action::Highlight)]);
    EXPECT_TRUE(edges.released[static_cast<size_t>(Action::Highlight)]);

    previous.actions[static_cast<size_t>(Action::Highlight)] = true;
    current.actions[static_cast<size_t>(Action::Highlight)] = true;
    edges = ComputeInputEdges(previous, current);
    EXPECT_FALSE(edges.pressed[static_cast<size_t>(Action::Highlight)]);
    EXPECT_FALSE(edges.released[static_cast<size_t>(Action::Highlight)]);
}