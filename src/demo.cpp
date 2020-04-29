#include "Game/demo.hpp"
#include "GE/Core/Parsers/objParser.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/LowRenderer/Light/directionnalLight.hpp"
#include "GE/LowRenderer/Light/pointLight.hpp"
#include "GE/LowRenderer/Light/spotLight.hpp"
#include "GE/Resources/text.hpp"
#include "GE/LowRenderer/billBoard.hpp"
#include "GE/Resources/type.hpp"

#include <SDL2/SDL_mouse.h>
#include "glad/glad.h"

using namespace Game;
using namespace Engine::Resources;
using namespace Engine::Core::Parsers;
using namespace Engine::Core::Maths;
using namespace Engine::Core::DataStructure;
using namespace Engine::LowRenderer;


Demo::Demo(Engine::GE& gameEngine)
    :   gameEngine_         (gameEngine),
        scene_              (),
        flagleftClicIsDown  (false),
        flagF1IsDown        (false),
        mouseForPlayer1     (true)
{
    loadResourceAndScene();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    SDL_ShowCursor(false);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
}


void Demo::update     () noexcept
{
    updateControl (gameEngine_.inputManager_, gameEngine_.getDeltaTime() * 3.f);       

    scene_.update();
}

void Demo::display    () const noexcept
{
    Size sizeWin = gameEngine_.getWinSize();

    glViewport(0, 0, sizeWin.width, sizeWin.heigth);
    static_cast<Camera*>(mainCamera->entity.get())->use();
    scene_.draw();
    
    glUseProgram(0);
}


void Demo::loadResourceAndScene  ()
{
    loadGeneralRessource   (gameEngine_.resourceManager_);

    //Load Camera
    Camera  camP1Arg ( {0.f, 10.f, 50.f},
                    {0.f, 0.f, 0.f}, 
                    gameEngine_.getWinSize().width / static_cast<float>(gameEngine_.getWinSize().heigth / 2.f), 0.1f, 10000.0f, 45.0f, "MainCamera");

    mainCamera = &scene_.add<Camera>(scene_.getWorld(), camP1Arg);

}

void Demo::loadGeneralRessource   (Resources& resourceManager)
{

}

void Demo::updateControl          (const Engine::Core::InputSystem::Input& input, float detlaTime)
{
 
}
