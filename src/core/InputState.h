#pragma once
#include <array>
#include <cstdint>
#include <cstddef>

namespace porting_base
{

    enum class Action : uint8_t
    {
        Quit,
        Highlight,
        Count
    };

    enum class Axis : uint8_t
    {
        MoveX,
        MoveY,
        Count
    };

    struct InputState
    {
        std::array<bool, static_cast<size_t>(Action::Count)> actions { };
        std::array<float, static_cast<size_t>(Axis::Count)> axes { };
        bool controllerConnected { false };
    };

    struct InputEdges
    {
        std::array<bool, static_cast<size_t>(Action::Count)> pressed {};
        std::array<bool, static_cast<size_t>(Action::Count)> released {};
    };

    InputEdges ComputeInputEdges(const InputState& previous, const InputState& current);
    float NormalizeAxis(int16_t raw, float deadzone = 0.24f);

    inline bool IsHeld(const InputState& state, Action action)
    {
        return state.actions[static_cast<size_t>(action)];
    }

    inline float AxisValue(const InputState& state, Axis axis)
    {
        return state.axes[static_cast<size_t>(axis)];
    }

    inline bool WasPressed(const InputEdges& edges, Action action)
    {
        return edges.pressed[static_cast<size_t>(action)];
    }
}