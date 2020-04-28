#include "rendererSDLOpGl33.hpp"
#include "log.hpp"
#include "windowSDL.hpp"
#include <SDL2/SDL_hints.h>

using namespace Engine::Core::Renderer;
using namespace Engine::Core::Debug;
using namespace std;

RendererSDLOpGl33::~RendererSDLOpGl33 ()
{
    release (); //RAII
}

void    RendererSDLOpGl33::initialize  (const WindowCreateArg& winArg)
{
    SLog::logInitializationStart("SDL/OpenGL 3.3 Renderer");

    //Init window
    pWin = std::make_unique<WindowSDL>();
    pWin->initialize(winArg);

    //Init openGL
    //init parameter :
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    

    openGlContext_ = SDL_GL_CreateContext((SDL_Window*)pWin->get());
    if(openGlContext_ == NULL)
    {
        functError((std::string("Failed to initialize openGl context : ") + SDL_GetError()).c_str());
    }

    //Init Glad (openGl 3.3 function)
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        functError("Failed to initialize GLAD");
    }

    SLog::logInitializationEnd("SDL/OpenGL 3.3 Renderer");
}

void   RendererSDLOpGl33::release   ()
{
    SDL_GL_DeleteContext(openGlContext_);
    SLog::log("SDL/OpenGL 3.3 Renderer release");
}