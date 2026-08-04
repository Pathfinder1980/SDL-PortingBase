#include "Platform.h"

#include <format>
#include <SDL.h>
#include <SDL_opengl.h>

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
        SDLGuard Guard;
        WindowPtr Window;
        ContextPtr GLContext;
        Uint64 performanceFrequency { 0 };
        InputState inputState {};
        bool IsQuitRequested { false };
    };    

    Platform::Platform() : pImpl(std::make_unique<Impl>())
    {
    }

    Platform::~Platform() = default;

    std::unique_ptr<Platform> Platform::Create(const WindowConfig& config, std::string& errorMessage)
    {
        std::unique_ptr<Platform> platform { new Platform() };
        Impl& pImpl = *platform->pImpl;
        if (SDL_Init(SDL_INIT_VIDEO))
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
        pImpl.Window = WindowPtr {
            SDL_CreateWindow(config.WindowName.c_str(), 
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                config.WindowWidth, config.WindowHeight, 
                WINDOW_FLAGS) };

        if (pImpl.Window == nullptr)
        {
            errorMessage = std::format("Error Create Window: {}", SDL_GetError());
            return nullptr;
        }

        pImpl.GLContext = ContextPtr { SDL_GL_CreateContext(pImpl.Window.get()) };
        if (pImpl.GLContext == nullptr)
        {
            errorMessage = std::format("Error Create GLContext: {}", SDL_GetError());
            return nullptr;
        }
        SDL_Log("OpenGL: %s", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        
        if (SDL_GL_SetSwapInterval(config.VSync ? 1 : 0) != 0)
        {
            SDL_Log("Warning Set Swap Interval: %s", SDL_GetError());
        }

        pImpl.performanceFrequency = SDL_GetPerformanceFrequency();
        
        return platform;
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
                    pImpl->IsQuitRequested = true;
                    break;
                }               

                default:
                {
                    break;
                }
            }
        }

        for (int i = 0; i < static_cast<int>(Action::Count); ++i)
        {
            pImpl->inputState.actions[i] = false;
        }
        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
        for (const KeyBinding& binding : kKeyBindings)
        {
            if (keyboardState[binding.scancode])
            {
                SetHeld(pImpl->inputState, binding.action, true);
            }
        }

        return pImpl->inputState;
    }

    void Platform::SetTitle(const std::string& title)
    {
        SDL_SetWindowTitle(pImpl->Window.get(), title.c_str());
    }

    void Platform::SwapBuffers()
    {
        SDL_GL_SwapWindow(pImpl->Window.get());
    }

    void Platform::ClearBuffer(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Platform::RequestQuit()
    {
        pImpl->IsQuitRequested = true;
    }

    bool Platform::IsQuitRequested() const
    {
        return pImpl->IsQuitRequested;
    }
} // namespace porting_base