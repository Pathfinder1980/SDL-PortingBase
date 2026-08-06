#include "renderer/GLApi.h"

namespace porting_base
{
    #define PB_GL_LOAD(name)                                                \
    do {                                                                    \
        name = reinterpret_cast<decltype(name)>(getProc("gl" #name));       \
        if (name == nullptr)                                                \
        {                                                                   \
            if (missingFunction != nullptr)                                 \
            {                                                               \
                *missingFunction = "gl" #name;                              \
            }                                                               \
            return false;                                                   \
        }                                                                   \
    } while (false)

    bool GLApi::Load(GetProcFn getProc, const char** missingFunction)
    {
        PB_GL_LOAD(ClearColor);
        PB_GL_LOAD(Clear);
        PB_GL_LOAD(Viewport);
        PB_GL_LOAD(GetString);

        return true;
    }

    #undef PB_GL_LOAD(name)
}