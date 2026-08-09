#include <memory>

#include "core/FrameTimer.h"
#include "core/FrameStats.h"
#include "core/InputState.h"
#include "core/AppEntry.h"
#include "platform/Platform.h"
#include "renderer/Renderer.h"

#include <string>
#include <random>
#include <filesystem>
#include <format>
#include <algorithm>
#include <cstdio>


namespace porting_base{
    
    namespace 
    {        
        constexpr const char* kWindowName = "SDL BasePortingFramework";
        constexpr int kWindowWidth = 640 ;
        constexpr int kWindowHeight = 480;
        constexpr double kFixedStep = 1.0 / 60.0;
        constexpr float kSpinSpeedX = 0.9f;
        constexpr float kSpinSpeedY = 0.6f;
    }
    


    int AppMain([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
    {
        WindowConfig config { kWindowName, kWindowWidth, kWindowHeight, true };
        std::string errorMessage;
        auto platform = Platform::Create(config, errorMessage);
        if (!platform)
        {
            std::fprintf(stderr, "%s\n", errorMessage.c_str());
            return 1;
        }

        const std::string& basePath = platform->GetBasePath(errorMessage);
        if (basePath.empty())
        {
            std::fprintf(stderr, "%s\n", errorMessage.c_str());
            return 1;
        }
        std::filesystem::path shaderPath = std::filesystem::path(basePath) / "assets" / "shaders";        

        auto renderer = Renderer::Create(platform->GetGLLoader(), shaderPath, errorMessage);
        if (!renderer)
        {
            std::fprintf(stderr, "%s\n", errorMessage.c_str());
            return 1;
        }
        std::printf("OpenGL: %s\n", renderer->GetVersionString().c_str());
        
        std::mt19937 rng { std::random_device { }() };
        float red { 0.09f }, green { 0.10f }, blue { 0.12f};
        FrameTimer timer { kFixedStep, 0.25 };
        
        double lastTime = platform->Now();
        float angleX { 0.f}, angleY { 0.f };
        FrameStats stats;
        InputState previousInputState;
        while (platform->IsQuitRequested() == false)
        {
            const InputState& currentInputState = platform->PumpEvents();
            const InputEdges inputEdges = ComputeInputEdges(previousInputState, currentInputState);
            previousInputState = currentInputState;

            double nowSeconds = platform->Now();
            double frameSeconds = nowSeconds - lastTime;
            lastTime = nowSeconds;
            stats.Update(frameSeconds);        
            if (stats.GetAccumulatedFrameTimeInSeconds() >= 0.5)
            {
                double fps = stats.GetAverageFPS();
                double frameMs = stats.GetAverageFrameTimeInMilliseconds();
                float xAxis = currentInputState.axes[static_cast<int>(Axis::MoveX)];
                float yAxis = currentInputState.axes[static_cast<int>(Axis::MoveY)];
                platform->SetTitle(std::format("{} | {:.1f} fps / {:.2f} ms | Controller: {} , | ({:.2f}, {:.2f})" , kWindowName, fps, frameMs, currentInputState.controllerConnected, xAxis, yAxis));
                stats.Reset();
            }
            
            int steps = timer.Advance(frameSeconds);
            while (steps > 0)
            {
                // Update game logic here
                angleX += static_cast<float>(kSpinSpeedX * kFixedStep);
                angleY += static_cast<float>(kSpinSpeedY * kFixedStep);
                steps--;
            }

            if (currentInputState.controllerConnected)
            {
                float axisX = currentInputState.axes[static_cast<int>(Axis::MoveX)];
                float axisY = -currentInputState.axes[static_cast<int>(Axis::MoveY)];
                const float dt = static_cast<float>(frameSeconds);
                red += axisY * dt;
                green += axisY * dt;
                blue += axisY * dt;

                green += axisX * dt;

                red = std::clamp(red, 0.f, 1.f);
                green = std::clamp(green, 0.f, 1.f);
                blue = std::clamp(blue, 0.f, 1.f);
            }
            
            if (WasPressed(inputEdges, Action::Highlight))
            {
                std::uniform_real_distribution<float> dist { 0.f, 1.f };
                red = dist(rng);
                green = dist(rng);
                blue = dist(rng);
            }

            if (WasPressed(inputEdges, Action::Quit))
            {
                platform->RequestQuit();
            }


            renderer->Clear(red, green, blue, 1.f);
            int width { 0 }, height { 0 };
            platform->GetDrawableSize(width, height);
            renderer->Draw(width, height, angleX, angleY);
            platform->SwapBuffers();
        }
        
        return 0;
    }
}