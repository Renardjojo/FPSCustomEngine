#include "GE/Core/Rendering/Renderer/rendererSDLOpGl33.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/Rendering/Window/windowSDL.hpp"
#include <SDL2/SDL_hints.h>

#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace Engine::Core::Renderer;
using namespace Engine::Core::Debug;
using namespace std;

RendererSDLOpGl33::~RendererSDLOpGl33 ()
{
    release (); //RAII
}

void    RendererSDLOpGl33::initialize  (const WindowCreateArg& winArg)
{
    SLog::logInitializationStart("SDL/OpenGL 4.6 Renderer");

    //Init window
    pWin = std::make_unique<WindowSDL>();
    pWin->initialize(winArg);

    //Init openGL
    //init parameter :
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
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

    SLog::logInitializationEnd("SDL/OpenGL 4.6 Renderer");

    SLog::logInitializationStart("init IMGUI for SDL2/OpenGL 4.6");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)pWin->get(), openGlContext_);
    const char* glsl_version = "#version 330 core";
    ImGui_ImplOpenGL3_Init(glsl_version);

    SLog::logInitializationEnd("init IMGUI for SDL2/OpenGL 4.6");
}

void   RendererSDLOpGl33::release   ()
{
    SDL_GL_DeleteContext(openGlContext_);
    SLog::log("SDL/OpenGL 4.6 Renderer release");
}