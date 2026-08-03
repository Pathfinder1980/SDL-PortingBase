#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "core/AppEntry.h"

using namespace porting_base;

int main(int argc, char** argv)
{
    SDL_SetMainReady();
    return AppMain(argc, argv);
}