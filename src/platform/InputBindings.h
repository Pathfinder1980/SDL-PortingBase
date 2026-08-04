#pragma once

#include <SDL_scancode.h>
#include "core/InputState.h"

namespace porting_base
{
    struct KeyBinding
    {
        SDL_Scancode scancode;
        Action action;
    };

    constexpr KeyBinding kKeyBindings[] = {
        { SDL_SCANCODE_ESCAPE, Action::Quit },
        { SDL_SCANCODE_SPACE, Action::Highlight },
    };
}