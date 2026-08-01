#define SDL_MAIN_HANDLED

#include <memory>
#include <SDL.h>
#include <SDL_opengl.h>

constexpr const char* WINDOW_NAME = "SDL BasePortingFramework";
constexpr int WINDOW_SIZE_WIDTH = 480;
constexpr int WINDOW_SIZE_HEIGHT = 320;
constexpr uint32_t WINDOW_FLAGS = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

enum class ErrorCode {
    ERROR_SDL_Init          = 1,
    ERROR_SDL_CreateWindow  = 2,
    ERROR_SDL_CreateContext = 3,
};

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

int main()
{
    SDL_SetMainReady();

    SDLGuard Guard;
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        const char* errorMessage = SDL_GetError();
        SDL_Log("Error Initializing SDL: {%s}", errorMessage);
        return static_cast<int>(ErrorCode::ERROR_SDL_Init);
    }

    SetGLAttribute(SDL_GL_DEPTH_SIZE, 24);
    SetGLAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SetGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SetGLAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SetGLAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#if !defined(NDEBUG)
    SetGLAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    WindowPtr Window { 
        SDL_CreateWindow(WINDOW_NAME, 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT, 
            WINDOW_FLAGS) 
    };
    if (Window == nullptr)
    {
        const char* errorMessage = SDL_GetError();
        SDL_Log("Error Create Window: {%s}", errorMessage);
        return static_cast<int>(ErrorCode::ERROR_SDL_CreateWindow);
    }

    ContextPtr GLContext { SDL_GL_CreateContext(Window.get()) };
    if (GLContext == nullptr)
    {
        const char* errorMessage = SDL_GetError();
        SDL_Log("Error Create GLContext: {%s}", errorMessage);
        return static_cast<int>(ErrorCode::ERROR_SDL_CreateContext);
    }
    SDL_Log("OpenGL: %s", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    if (SDL_GL_SetSwapInterval(1))
    {
        const char* errorMessage = SDL_GetError();
        SDL_Log("Warning Set Swap Interval: {%s}", errorMessage);
    }

    bool ApplicationRunning { true };
    while (ApplicationRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                {
                    ApplicationRunning = false;
                    break;
                }

                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    {
                        ApplicationRunning = false;                        
                    }
                    break;
                }

                default:
                {
                    break;
                }
            }
        }

        glClearColor(0.09f, 0.10f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapWindow(Window.get());
    }

    return 0;
}