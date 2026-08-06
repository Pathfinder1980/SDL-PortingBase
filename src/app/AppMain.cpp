#include <memory>

#include "core/FrameTimer.h"
#include "core/FrameStats.h"
#include "core/InputState.h"
#include "platform/Platform.h"
#include "renderer/Renderer.h"
#include <string>

#include "core/AppEntry.h"
#include <random>

namespace porting_base{
    constexpr const char* WINDOW_NAME = "SDL BasePortingFramework";
    constexpr int WINDOW_SIZE_WIDTH = 640 ;
    constexpr int WINDOW_SIZE_HEIGHT = 480;
    
    int AppMain([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
    {
        WindowConfig config { WINDOW_NAME, WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT, true };
        std::string errorMessage;
        auto platform = Platform::Create(config, errorMessage);
        if (!platform)
        {
            std::fprintf(stderr, "%s\n", errorMessage.c_str());
            return 1;
        }

        auto renderer = Renderer::Create(platform->GetGLLoader(), errorMessage);
        if (!renderer)
        {
            std::fprintf(stderr, "%s\n", errorMessage.c_str());
            return 1;
        }
        std::printf("OpenGL: %s\n", renderer->GetVersionString().c_str());
        
        std::mt19937 rng { std::random_device { }() };
        float red { 0.09f }, green { 0.10f }, blue { 0.12f};
        FrameTimer timer { 1.0 / 60.0, 0.25 };
        
        double lastTime = platform->Now();
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

                char title[128];
                std::snprintf(title, sizeof(title), "%s | %.1f fps / %.2f ms | Controller: %i | (%.2f, %.2f)", 
                    WINDOW_NAME, fps, frameMs, 
                    currentInputState.controllerConnected, 
                    currentInputState.axes[static_cast<int>(Axis::MoveX)],
                    currentInputState.axes[static_cast<int>(Axis::MoveY)]);
                platform->SetTitle(title);
                stats.Reset();
            }
            
            int steps = timer.Advance(frameSeconds);
            while (steps > 0)
            {
                // Update game logic here
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
            renderer->Draw();
            platform->SwapBuffers();
        }
        
        return 0;
    }
}