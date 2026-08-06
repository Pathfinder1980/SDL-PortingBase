#include "renderer/GLApi.h"

namespace porting_base
{
    bool GLApi::Load(GetProcFn getProc, const char** missingFunction)
    {
        ClearColor = reinterpret_cast<decltype(ClearColor)>(getProc("glClearColor"));
        if (ClearColor == nullptr)
        {
            if (missingFunction)
            {
                *missingFunction = "glClearColor";
            }
            return false;
        }

        Clear = reinterpret_cast<decltype(Clear)>(getProc("glClear"));
        if (Clear == nullptr)
        {
            if (missingFunction)
            {
                *missingFunction = "glClear";
            }
            return false;
        }

        Viewport = reinterpret_cast<decltype(Viewport)>(getProc("glViewport"));
        if (Viewport == nullptr)
        {
            if (missingFunction)
            {
                *missingFunction = "glViewport";
            }
            return false;
        }

        GetString = reinterpret_cast<decltype(GetString)>(getProc("glGetString"));
        if (GetString == nullptr)
        {
            if (missingFunction)
            {
                *missingFunction = "glGetString";
            }
            return false;
        }


        return true;
    }
}