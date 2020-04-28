#include "GE.hpp"
#include "log.hpp"
#include "rendererSDLOpGl33.hpp"

using namespace Engine;
using namespace Engine::Core::Debug;
using namespace Engine::Core::InputSystem;


GE::GE ()
{
    //init Log
    SLog::init();

    //init renderer
    ren_ = std::make_unique<Engine::Core::Renderer::RendererSDLOpGl33>();

    Engine::Core::Renderer::WindowCreateArg winArg;
    winArg.name     = "Window";
    winArg.height   = 800;
    winArg.width    = 1000;

    ren_->initialize(winArg);

    //init random value
    srand(time(NULL));
}

GE::~GE ()
{
    SLog::release();
}
