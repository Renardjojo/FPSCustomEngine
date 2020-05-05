#include "Game/demo.hpp"
#include "GE/Core/Parsers/objParser.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/LowRenderer/Light/directionnalLight.hpp"
#include "GE/LowRenderer/Light/pointLight.hpp"
#include "GE/LowRenderer/Light/spotLight.hpp"
#include "GE/Ressources/text.hpp"
#include "GE/LowRenderer/billBoard.hpp"
#include "GE/Ressources/type.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Core/System/UISystem.hpp"
#include "GE/Ressources/ui.hpp"
#include "Game/define.h"

#include "../src/stb_image.h"


#include <SDL2/SDL_mouse.h>
#include "glad/glad.h"

using namespace Game;
using namespace Engine;
using namespace Engine::Physics;
using namespace Engine::Ressources;
using namespace Engine::LowRenderer;
using namespace Engine::Core::Time;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Parsers;
using namespace Engine::Core::Systems;
using namespace Engine::Core::DataStructure;


Demo::Demo(Engine::GE& gameEngine)
    :   gameEngine_         (gameEngine),
        scene_              (),
        flagleftClicIsDown  (false),
        flagF1IsDown        (false),
        usingMouse          (true),
        dirCamera          {0.f, 0.f, -1.f}
{

    loadRessourceAndScene();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    SDL_ShowCursor(false);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    UISystem::isActive = true;

}


void Demo::update     () noexcept
{
    UISystem::update(gameEngine_);
    updateControl(gameEngine_.inputManager_);
 
    if (gameEngine_.gameState == E_GAME_STATE::RUNNING)
    {
        PhysicSystem::update();

        scene_.update();
    }
}

void Demo::display    () const noexcept
{
    Size sizeWin = gameEngine_.getWinSize();

    if (gameEngine_.gameState == E_GAME_STATE::RUNNING || gameEngine_.gameState == E_GAME_STATE::PAUSE)
    {
        glViewport(0, 0, sizeWin.width, sizeWin.heigth);
        static_cast<Camera*>(mainCamera->entity.get())->use();
        
        scene_.draw();
    }
    
    UISystem::draw(gameEngine_);
    
    glUseProgram(0);
}


void Demo::loadRessourceAndScene  ()
{
    loadGeneralRessource   (gameEngine_.ressourceManager_);
    
    loadUI(gameEngine_.ressourceManager_);

    //Load Camera
    Camera  camP1Arg ({0.f, 0.f, 15.f},
                    {0.f, 0.f, 0.f}, 
                    gameEngine_.getWinSize().width / static_cast<float>(gameEngine_.getWinSize().heigth), 0.1f, 10000.0f, 45.0f, "MainCamera");


    mainCamera = &scene_.add<Camera>(scene_.getWorld(), camP1Arg);
                functGlCheckAndLogError();

}

void Demo::loadGeneralRessource   (Ressources& ressourceManager)
{
    MaterialAndTextureCreateArg matDefault;
    matDefault.name_                = "DefaultMaterial";
    matDefault.pathDiffuseTexture   = nullptr;
    matDefault.flipTexture          = false;

    ModelCreateArg cube1arg     {{0.f, -5.f, 0.f}, 
                                {0.f, 0.f, 0.f}, 
                                {5.f, 0.1f, 5.f}, 
                                &ressourceManager.add<Shader>("White", "./ressources/shader/vLightObj.vs", "./ressources/shader/fLightObj.fs"), 
                                {&ressourceManager.add<Material>("DefaultMaterial", matDefault)}, 
                                &ressourceManager.add<Mesh>("cube1", Mesh::createCube(1)),
                                "cube1"};

    scene_.add<Model>(scene_.getWorld(), cube1arg);

    ModelCreateArg sphere1arg   {{0.f, 5.f, 0.f}, 
                                {0.f, 0.f, 0.f}, 
                                {0.5f, 0.5f, 0.5f}, 
                                &ressourceManager.get<Shader>("White"), 
                                {&ressourceManager.get<Material>("DefaultMaterial")}, 
                                &ressourceManager.add<Mesh>("sphere1", Mesh::createSphere(25,25)),
                                "sphere1"};

    scene_.add<Model>(scene_.getWorld(), sphere1arg);

    scene_.getGameObject("world/sphere1").addComponent<PhysicalObject>();

    scene_.getGameObject("world/sphere1").getComponent<PhysicalObject>()->SetMass(10);

    scene_.getGameObject("world/sphere1").addComponent<SphereCollider>();
    scene_.getGameObject("world/sphere1").getComponent<SphereCollider>()->SetBounciness(0.5f);
    scene_.getGameObject("world/cube1").addComponent<OrientedBoxCollider>();

                    functGlCheckAndLogError();

}

void Demo::loadUI      (Ressources& ressourceManager)
{
    FontCreateArg fontarg {"./ressources/opensans.ttf", 40};
    Font * pfont = &ressourceManager.add<Font>("font1", fontarg);
    Shader* buttonShader = &ressourceManager.add<Shader>("ButtonShader", "./ressources/shader/text.vs", "./ressources/shader/texture.fs");

    int tempX = gameEngine_.getWinSize().width / 2.0f;
    int tempY = gameEngine_.getWinSize().heigth / 2.0f;



    ressourceManager.add<Button>("MenuStartButton", pfont, buttonShader, 
                                            tempX - 35, tempY - 200, 
                                            150.0f, 60.0f, SDL_Color{170, 80, 80, 0}, "Start",
                                            E_GAME_STATE::STARTING).function = [&]()
    {
        gameEngine_.gameState = E_GAME_STATE::RUNNING;
        usingMouse = false;
    };

    ressourceManager.add<Button>("MenuLoadButton", pfont, buttonShader, 
                                            tempX - 35, tempY - 100, 
                                            150.0f, 60.0f, SDL_Color{170, 170, 80, 0}, "Load",
                                            E_GAME_STATE::STARTING).function = [&]()
    {
        
    };

    ressourceManager.add<Button>("MenuOptionButton", pfont, buttonShader, 
                                            tempX - 55, tempY, 
                                            150.0f, 60.0f, SDL_Color{80, 170, 170, 0}, "Option",
                                            E_GAME_STATE::STARTING).function = [&]()
    {
        
    };

    ressourceManager.add<Button>("MenuQuitButton", pfont, buttonShader,
                                            tempX - 35, tempY + 100, 
                                            150.0f, 60.0f, SDL_Color{80, 80, 170, 0}, "Quit",
                                            E_GAME_STATE::STARTING).function = [&]()
    {
        gameEngine_.gameState = E_GAME_STATE::EXIT;
    };
    ressourceManager.add<Button>("PausePlayButton", pfont, buttonShader,
                                            tempX - 35, tempY - 100, 
                                            150.0f, 60.0f, SDL_Color{170, 80, 170, 0}, "Play",
                                            E_GAME_STATE::PAUSE).function = [&]()
    {
        gameEngine_.gameState = E_GAME_STATE::RUNNING;
        usingMouse = false;
    };
    ressourceManager.add<Button>("PauseMenuButton", pfont, buttonShader,
                                            tempX - 45, tempY, 
                                            150.0f, 60.0f, SDL_Color{80, 170, 80, 0}, "Menu",
                                            E_GAME_STATE::PAUSE).function = [&]()
    {
        gameEngine_.gameState = E_GAME_STATE::STARTING;
        usingMouse = true;
    };

}

void Demo::loadLights      (Ressources& ressourceManager)
{
    ModelCreateArg lightArg     {{0.f, 0.f, 0.f},
                                {0.f, 0.f, 0.f},
                                {1.f, 1.f, 1.f},
                                &ressourceManager.get<Shader>("White"),
                                {&ressourceManager.get<Material>("DefaultMaterial")},
                                &ressourceManager.get<Mesh>("Sphere"),
                                "Ground",
                                true};

    DirectionnalLightCreateArg lightArg2 {   {0.f, 1.f, 1.f},
                                        {1.f, 1.f, 1.f, 0.1f},
                                        {1.f, 1.f, 1.f, 0.7f},
                                        {1.f, 1.f, 1.f, 1.f}, "light"};

    PointLightCreateArg lightArg5 { {50.f, 20.f, -50.f},
                                    {1.f, 1.f, 1.f, 0.f},
                                    {0.f, 1.f, 0.f, 0.7f},
                                    {1.f, 1.f, 1.f, 0.3f},
                                    0.f, 0.05f, 0.f, "light"};

    sunLight = &scene_.add<DirectionnalLight>(scene_.getWorld(), lightArg2);

    GameObject& pl1 = scene_.add<PointLight>(scene_.getWorld(), lightArg5);

    scene_.add<Model>(pl1, lightArg);

    static_cast<DirectionnalLight*>(sunLight->entity.get())->enable(true);
    static_cast<PointLight*>(pl1.entity.get())->enable(true);
}

void Demo::updateControl(Engine::Core::InputSystem::Input& input)
{
    if (input.keyboard.isDown[SDL_SCANCODE_UP])
    {
        Vec3 vec = dirCamera * (20.f * TimeSystem::getDeltaTime());
        static_cast<Camera*>(mainCamera->entity.get())->translate(vec);
    }

    if (input.keyboard.isDown[SDL_SCANCODE_DOWN])
    {
        Vec3 vec = dirCamera * (20.f * TimeSystem::getDeltaTime());
        static_cast<Camera*>(mainCamera->entity.get())->translate(-vec);
    }

    if (input.keyboard.isDown[SDL_SCANCODE_LEFT])
    {
        Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
        vecDirPlayer.rotate(-input.mouse.motion.x * 0.1f * TimeSystem::getDeltaTime() * 180 / M_PI).rotated90();
        Vec3 playerDirOrtho {vecDirPlayer.x, dirCamera.y, vecDirPlayer.y};

        playerDirOrtho *= (20.f * TimeSystem::getDeltaTime());
        static_cast<Camera*>(mainCamera->entity.get())->translate(-playerDirOrtho);
    }

    if (input.keyboard.isDown[SDL_SCANCODE_RIGHT])
    {
        Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
        vecDirPlayer.rotate(-input.mouse.motion.x * 0.1f * TimeSystem::getDeltaTime() * 180 / M_PI).rotated90();
        Vec3 playerDirOrtho {vecDirPlayer.x, dirCamera.y, vecDirPlayer.y};

        playerDirOrtho *= (20.f * TimeSystem::getDeltaTime());
        static_cast<Camera*>(mainCamera->entity.get())->translate(playerDirOrtho);
    }

    if (input.mouse.motion.y != 0)
    {
        if (!usingMouse)
        {
            static_cast<Camera*>(mainCamera->entity.get())->rotate(-input.mouse.motion.y * 0.1f * TimeSystem::getDeltaTime(), {1.f, 0.f, 0.f});
        }
    }

    if (input.mouse.motion.x != 0)
    {
        if (!usingMouse)
        {
            Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
            vecDirPlayer.rotate(input.mouse.motion.x * 0.1f * TimeSystem::getDeltaTime() * 180 / M_PI);
            dirCamera.x = vecDirPlayer.x;
            dirCamera.z = vecDirPlayer.y;
            static_cast<Camera*>(mainCamera->entity.get())->rotate(-input.mouse.motion.x * 0.1f * TimeSystem::getDeltaTime(), {0.f, 1.f, 0.f});
        }
    }

    static int exFrameWheelVal = input.mouse.wheel_scrolling;

    if (input.mouse.wheel_scrolling != exFrameWheelVal)
    {
        if(input.mouse.wheel_scrolling > exFrameWheelVal)
        {
            static_cast<Camera*>(mainCamera->entity.get())->setFovY(static_cast<Camera*>(mainCamera->entity.get())->getProjectionInfo().fovY + 5);
        }
        else
        {
            static_cast<Camera*>(mainCamera->entity.get())->setFovY(static_cast<Camera*>(mainCamera->entity.get())->getProjectionInfo().fovY - 5);
        }
        
        exFrameWheelVal = input.mouse.wheel_scrolling;
    }

    if (input.keyboard.onePressed(SDL_SCANCODE_ESCAPE) == 1)
    {
        if (gameEngine_.gameState == E_GAME_STATE::RUNNING)
        {
            gameEngine_.gameState = E_GAME_STATE::PAUSE;
            usingMouse = true;
        }
        else if (gameEngine_.gameState == E_GAME_STATE::PAUSE)
        {
            gameEngine_.gameState = E_GAME_STATE::RUNNING;
            usingMouse = false;
        }
        else if (gameEngine_.gameState == E_GAME_STATE::STARTING)
        {
            gameEngine_.gameState = E_GAME_STATE::EXIT;
        }
    }

    if (input.keyboard.isDown[SDL_SCANCODE_F1] && !flagF1IsDown)
    {
        usingMouse = !usingMouse;
        flagF1IsDown = true;
    }
    else
    {
        flagF1IsDown = input.keyboard.isDown[SDL_SCANCODE_F1];
    }    
    if (!usingMouse)
    {
        SDL_ShowCursor(false);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    else
    {
        SDL_ShowCursor(true);
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
    
}