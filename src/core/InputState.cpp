#include "core/InputState.h"

#include <algorithm>
#include <limits>
#include <cmath>

namespace porting_base
{

    InputEdges ComputeInputEdges(const InputState &previous, const InputState &current)
    {
        InputEdges edges;
        for (size_t i = 0; i < static_cast<size_t>(Action::Count); ++i)
        {
            edges.pressed[i] = current.actions[i] && !previous.actions[i];
            edges.released[i] = !current.actions[i] && previous.actions[i];
        }
        return edges;
    }

    float NormalizeAxis(int16_t raw, float deadzone)
    {
        float transformed = std::clamp<float>(raw / static_cast<float>(std::numeric_limits<int16_t>::max()), -1.f, 1.f);
        const float magnitude = std::fabs(transformed);
        if (magnitude < deadzone)
        {
            return 0.0f;
        }

        return std::copysign((magnitude - deadzone) / (1.0f - deadzone), transformed);
    }

}