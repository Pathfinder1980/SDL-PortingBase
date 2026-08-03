#pragma once

#include <memory>
#include <string>

namespace porting_base
{
    struct WindowConfig
    {
        std::string WindowName;
        int WindowWidth;
        int WindowHeight;
        bool VSync;
    };

    class Platform
    {
    public:
        ~Platform();

        static std::unique_ptr<Platform> Create(const WindowConfig& config, std::string& errorMessage);

        double Now() const;
        void PumpEvents();

        void SetTitle(const std::string& title);
        void SwapBuffers();
        void ClearBuffer(float r, float g, float b, float a);

        bool IsQuitRequested() const;

    private:
        Platform();

        struct Impl;
        std::unique_ptr<Impl> pImpl;
    };
} // namespace porting_base