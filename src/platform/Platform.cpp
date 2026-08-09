#include "Platform.h"

#include <format>
#include <SDL.h>

#include "InputBindings.h"

namespace porting_base
{
    namespace
    {
        struct SDLGuard 
        {
            ~SDLGuard() { SDL_Quit(); }
        };
        
        struct WindowDeleter
        {
            void operator()(SDL_Window* window) const { SDL_DestroyWindow(window); }
        };
        using WindowPtr = std::unique_ptr<SDL_Window, WindowDeleter>;
        
        struct ContextDeleter
        {
            void operator()(SDL_GLContext context) const { SDL_GL_DeleteContext(context); }
        };
        using ContextPtr = std::unique_ptr<void, ContextDeleter>;
        
        struct ControllerDeleter
        {
            void operator()(SDL_GameController* controller) const { SDL_GameControllerClose(controller); }
        };
        using ControllerPtr = std::unique_ptr<SDL_GameController, ControllerDeleter>;

        void SetGLAttribute(SDL_GLattr attr, int value)
        {
            if (SDL_GL_SetAttribute(attr, value))
            {
                const char* errorMessage = SDL_GetError();
                SDL_Log("Error SDL_GL_SetAttribute %d: {%s}", attr, errorMessage);
            }
        }
    } // namespace
    
    struct Platform::Impl
    {   
        SDLGuard guard;
        WindowPtr window;
        ContextPtr glContext;
        Uint64 performanceFrequency { 0 };
        InputState inputState {};
        ControllerPtr controller;
        bool isQuitRequested { false };
        std::string basePath {};
    };    

    Platform::Platform() : pImpl(std::make_unique<Impl>())
    {
    }

    Platform::~Platform() = default;

    std::unique_ptr<Platform> Platform::Create(const WindowConfig& config, std::string& errorMessage)
    {
        std::unique_ptr<Platform> platform { new Platform() };
        Impl& pImpl = *platform->pImpl;
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER))
        {            
            errorMessage = std::format("Error Initializing SDL: {}", SDL_GetError());
            return nullptr;
        }

        SetGLAttribute(SDL_GL_DEPTH_SIZE, 24);
        SetGLAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SetGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SetGLAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SetGLAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    #if !defined(NDEBUG)
        SetGLAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    #endif

        constexpr uint32_t WINDOW_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
        pImpl.window = WindowPtr {
            SDL_CreateWindow(config.WindowName.c_str(), 
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                config.WindowWidth, config.WindowHeight, 
                WINDOW_FLAGS) };

        if (pImpl.window == nullptr)
        {
            errorMessage = std::format("Error Create Window: {}", SDL_GetError());
            return nullptr;
        }

        pImpl.glContext = ContextPtr { SDL_GL_CreateContext(pImpl.window.get()) };
        if (pImpl.glContext == nullptr)
        {
            errorMessage = std::format("Error Create GLContext: {}", SDL_GetError());
            return nullptr;
        }
        
        
        if (SDL_GL_SetSwapInterval(config.VSync ? 1 : 0) != 0)
        {
            SDL_Log("Warning Set Swap Interval: %s", SDL_GetError());
        }

        pImpl.performanceFrequency = SDL_GetPerformanceFrequency();
        
        return platform;
    }

    GlGetProc Platform::GetGLLoader()
    {
        return reinterpret_cast<GlGetProc>(&SDL_GL_GetProcAddress);   
    }

    double Platform::Now() const
    {
        const Uint64 now = SDL_GetPerformanceCounter();
        double FrameSeconds = now / static_cast<double>(pImpl->performanceFrequency);
        return FrameSeconds;
    }

    const InputState& Platform::PumpEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                {
                    RequestQuit();
                    break;
                }
                
                case SDL_CONTROLLERDEVICEADDED:
                {                    
                    Sint32 deviceID { event.cdevice.which };
                    pImpl->controller.reset(SDL_GameControllerOpen(deviceID));
                    pImpl->inputState.controllerConnected = true;
                    SDL_Log("Controller: Connected!");
                    break;
                }
                
                case SDL_CONTROLLERDEVICEREMOVED:
                {                    
                    if (SDL_GameControllerFromInstanceID(event.cdevice.which) == pImpl->controller.get())
                    {
                        pImpl->controller.reset();
                        pImpl->inputState.controllerConnected = false;
                        SDL_Log("Controller: Disconnected!");
                    }                    
                    break;
                }
                    
                default:
                {
                    break;
                }

            }
        }

        pImpl->inputState.actions = {};
        pImpl->inputState.axes = {};
        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
        for (const KeyBinding& binding : kKeyBindings)
        {
            if (keyboardState[binding.scancode])
            {
                SetHeld(pImpl->inputState, binding.action, true);
            }
        }

        if (pImpl->inputState.controllerConnected)
        {
            for (const PadBinding& padBinding : kPadButtonBindings)
            {
                if (SDL_GameControllerGetButton(pImpl->controller.get(), padBinding.button))
                {
                    SetHeld(pImpl->inputState, padBinding.action, true);
                }
            }
            
            Sint16 axisRawX = SDL_GameControllerGetAxis(pImpl->controller.get(), SDL_CONTROLLER_AXIS_LEFTX);
            SetAxisValue(pImpl->inputState, Axis::MoveX, axisRawX);

            Sint16 axisRawY = SDL_GameControllerGetAxis(pImpl->controller.get(), SDL_CONTROLLER_AXIS_LEFTY);
            SetAxisValue(pImpl->inputState, Axis::MoveY, axisRawY);
        }

        return pImpl->inputState;
    }

    void Platform::SetTitle(const std::string& title)
    {
        SDL_SetWindowTitle(pImpl->window.get(), title.c_str());
    }

    void Platform::SwapBuffers()
    {
        SDL_GL_SwapWindow(pImpl->window.get());
    }    

    void Platform::RequestQuit()
    {
        pImpl->isQuitRequested = true;
    }

    bool Platform::IsQuitRequested() const
    {
        return pImpl->isQuitRequested;
    }

    const std::string& Platform::GetBasePath(std::string& errorMessage) const
    {
        if (!pImpl->basePath.empty())
        {
            return pImpl->basePath;
        }

        char* basePath = SDL_GetBasePath();
        if (basePath)
        {
            pImpl->basePath = basePath;
            SDL_free(basePath);
        }
        else
        {
            errorMessage = std::format("Error GetBasePath: {}", SDL_GetError());
        }
        return pImpl->basePath;
    }

    void Platform::GetDrawableSize(int& width, int& height) const
    {
        SDL_GL_GetDrawableSize(pImpl->window.get(), &width, &height);
    }
} // namespace porting_base