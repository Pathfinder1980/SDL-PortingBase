#pragma once

#include <memory>
#include <string>

#include "core/InputState.h"

namespace porting_base
{
    struct WindowConfig
    {
        std::string WindowName;
        int WindowWidth;
        int WindowHeight;
        bool VSync;
    };

    using GlGetProc = void* (*)(const char*);

    class Platform
    {
    public:
        ~Platform();

        static std::unique_ptr<Platform> Create(const WindowConfig& config, std::string& errorMessage);

        GlGetProc GetGLLoader();

        double Now() const;
        const InputState& PumpEvents();

        void SetTitle(const std::string& title);
        void SwapBuffers();

        void RequestQuit();
        bool IsQuitRequested() const;

    private:
        Platform();

        struct Impl;
        std::unique_ptr<Impl> pImpl;
    };
} // namespace porting_base