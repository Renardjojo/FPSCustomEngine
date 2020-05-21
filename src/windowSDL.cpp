#include "GE/Core/Rendering/Window/windowSDL.hpp"
#include "GE/Core/Debug/log.hpp"

using namespace Engine::Core::Renderer;
using namespace Engine::Core::Debug;
using namespace std;

WindowSDL::~WindowSDL ()
{
    release (); //RAII
}

void    WindowSDL::initialize (const WindowCreateArg& arg) noexcept
{
    SLog::logInitializationStart("Window");

    if (SDL_VideoInit(SDL_GetVideoDriver(SDL_GetNumVideoDrivers())) != 0)
    {
        SLog::logError((std::string("SDL initilization error") + SDL_GetError()).c_str());
        exit(EXIT_FAILURE);
    }

    win = SDL_CreateWindow( arg.name,
                            SDL_WINDOWPOS_CENTERED, 
                            SDL_WINDOWPOS_CENTERED, 
                            arg.width, 
                            arg.height, 
                            SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_GRABBED );

    if (win == NULL)
    {
        SLog::logError((std::string("window initilization error") + SDL_GetError()).c_str());
        exit(EXIT_FAILURE);
    }

    SLog::logInitializationEnd("Window");
}

void   WindowSDL::release   () noexcept
{
    SDL_DestroyWindow(win);
	SDL_VideoQuit();
    SLog::log("Window release");
}