#include <memory>

#include "core/FrameTimer.h"
#include "core/FrameStats.h"
#include "platform/Platform.h"
#include <string>

#include "core/AppEntry.h"

namespace porting_base{
    constexpr const char* WINDOW_NAME = "SDL BasePortingFramework";
    constexpr int WINDOW_SIZE_WIDTH = 480;
    constexpr int WINDOW_SIZE_HEIGHT = 320;
    
    int AppMain(int argc, char** argv)
    {
        WindowConfig config { WINDOW_NAME, WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT, true };
        std::string errorMessage;
        auto platform = Platform::Create(config, errorMessage);
        if (!platform)
        {
            std::fprintf(stderr, "%s\n", errorMessage.c_str());
            return 1;
        }
        
        FrameTimer timer { 1.0 / 60.0, 0.25 };
        
        double lastTime = platform->Now();
        FrameStats stats;    
        while (platform->IsQuitRequested() == false)
        {
            platform->PumpEvents();
            
            
            double nowSeconds = platform->Now();
            double FrameSeconds = nowSeconds - lastTime;
            lastTime = nowSeconds;
            stats.Update(FrameSeconds);        
            if (stats.GetAccumulatedFrameTimeInSeconds() >= 0.5)
            {
                double fps = stats.GetAverageFPS();
                double frameMs = stats.GetAverageFrameTimeInMilliseconds();
                char title[128];
                std::snprintf(title, sizeof(title), "%s | %.1f fps / %.2f ms", WINDOW_NAME, fps, frameMs);
                platform->SetTitle(title);
                stats.Reset();
            }
            
            int steps = timer.Advance(FrameSeconds);
            while (steps > 0)
            {
                // Update game logic here
                steps--;
            }
            
            platform->ClearBuffer(0.09f, 0.10f, 0.12f, 1.0f);
            platform->SwapBuffers();
        }
        
        return 0;
    }
}