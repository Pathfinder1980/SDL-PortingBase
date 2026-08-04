#pragma once

#include <SDL_scancode.h>
#include <SDL_gamecontroller.h>
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

    struct PadBinding
    {
        SDL_GameControllerButton button;
        Action action;
    };

    constexpr PadBinding kPadButtonBindings[] =
    {
        { SDL_CONTROLLER_BUTTON_A, Action::Highlight },
        { SDL_CONTROLLER_BUTTON_BACK, Action::Quit },
    };
}