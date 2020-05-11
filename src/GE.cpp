#include "GE/GE.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/Rendering/Renderer/rendererSDLOpGl33.hpp"
#include "Game/define.h"
#include "save/saves.hpp"

using namespace Ressoures::Save;

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
    winArg.width    = WIDTH;
    winArg.height   = HEIGHT;

    ren_->initialize(winArg);

    //init random value
    srand(time(NULL));
    
    gameState = E_GAME_STATE::STARTING;

    initSavePaths(savePaths, "./ressources/saves/LevelPaths");
}

GE::~GE ()
{
    SLog::release();
}
