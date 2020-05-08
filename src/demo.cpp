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
#include "save/xmlparse.hpp"
#include "Game/BarIndicatorController.hpp"

#include "../src/stb_image.h"


#include "GE/Core/System/ScriptSystem.hpp"
#include "Game/PlayerController.hpp"
#include "Game/EnnemyController.hpp"

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
using namespace Engine::Core::System;
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

    scene_ = std::make_unique<Scene>();

    loadRessources(gameEngine_.ressourceManager_);
    
    loadCamera(gameEngine_.ressourceManager_);

    loadEntity(gameEngine_.ressourceManager_);
    // setupScene(scene_, gameEngine_, "./ressources/saves/setup.xml");
    
    loadUI(gameEngine_.ressourceManager_);


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
        ScriptSystem::update();

        scene_->update();
    }
}

void Demo::display    () const noexcept
{
    Size sizeWin = gameEngine_.getWinSize();

    if (gameEngine_.gameState == E_GAME_STATE::RUNNING || gameEngine_.gameState == E_GAME_STATE::PAUSE)
    {
        glViewport(0, 0, sizeWin.width, sizeWin.heigth);
        static_cast<Camera*>(mainCamera->entity.get())->use();
        
        scene_->draw();
    }
    
    UISystem::draw(gameEngine_);
    
    glUseProgram(0);
}

void Demo::loadRessources(t_RessourcesManager &ressourceManager)
{
    ressourceManager.add<Shader>("White", "./ressources/shader/vLightObj.vs", "./ressources/shader/fLightObj.fs", AMBIANTE_COLOR_ONLY);

    MaterialAndTextureCreateArg matDefault;
    matDefault.name_ = "DefaultMaterial";
    matDefault.pathDiffuseTexture = nullptr;
    matDefault.flipTexture = false;
    ressourceManager.add<Material>("DefaultMaterial", matDefault);

    matDefault.name_ = "PinkMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{1.f, 0.f, 1.f, 1.f};
    ressourceManager.add<Material>(matDefault.name_, matDefault);

    matDefault.name_ = "BlackMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{0.f, 0.f, 0.f, 1.f};
    ressourceManager.add<Material>(matDefault.name_, matDefault);

    matDefault.name_ = "GreenMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{0.f, 1.f, 0.f, 1.f};
    ressourceManager.add<Material>(matDefault.name_, matDefault);

    ressourceManager.add<Mesh>("Cube1", Mesh::createCube(1));
    ressourceManager.add<Mesh>("Sphere1", Mesh::createSphere(25, 25));
    ressourceManager.add<Mesh>("Plane1", Mesh::createPlane());
}

void Demo::loadCamera(t_RessourcesManager &ressourceManager)
{
    Camera camP1Arg({0.f, 0.f, 30.f},
                {0.f, 0.f, 0.f},
                gameEngine_.getWinSize().width / static_cast<float>(gameEngine_.getWinSize().heigth), 0.1f, 10000.0f, 45.0f, "MainCamera");

    mainCamera = &scene_->add<Camera>(scene_->getWorld(), camP1Arg);
    static_cast<Camera *>(mainCamera->entity.get())->use();
}

void Demo::loadEntity(t_RessourcesManager &ressourceManager)
{
    ModelCreateArg cube1arg     {{0.f, -5.f, 0.f}, 
                                {0.f, 0.f, 0.f}, 
                                {5.f, 1.f, 5.f}, 
                                &ressourceManager.get<Shader>("White"), 
                                {&ressourceManager.get<Material>("DefaultMaterial")}, 
                                &ressourceManager.get<Mesh>("Cube1"),
                                "cube1"};

    scene_->add<Model>(scene_->getWorld(), cube1arg);

    ModelCreateArg sphere1arg{{0.f, 5.f, 0.f},
                              {0.f, 0.f, 0.f},
                              {1.0f, 1.0f, 1.0f},
                              &ressourceManager.get<Shader>("White"),
                              {&ressourceManager.get<Material>("PinkMaterial")},
                              &ressourceManager.get<Mesh>("Sphere1"),
                              "sphere1"};

    GameObject &sphere = scene_->add<Model>(scene_->getWorld(), sphere1arg);

    ModelCreateArg player{{0.f, 0.f, 0.f},
                          {0.f, 0.f, 0.f},
                          {1.0f, 1.0f, 1.0f},
                          &ressourceManager.get<Shader>("White"),
                          {&ressourceManager.get<Material>("DefaultMaterial")},
                          &ressourceManager.get<Mesh>("Sphere1"),
                          "Player"};

    scene_->add<Model>(scene_->getWorld(), player);
    scene_->getGameObject("world/Player").addComponent<PlayerController>(gameEngine_.inputManager_);
    scene_->getGameObject("world/Player").addComponent<PhysicalObject>();
    scene_->getGameObject("world/Player").getComponent<PhysicalObject>()->SetMass(1);
    scene_->getGameObject("world/Player").addComponent<SphereCollider>();
    scene_->getGameObject("world/Player").getComponent<SphereCollider>()->SetBounciness(0.1f);

    // playerGameObject.addComponent<PlayerController>(gameEngine_.inputManager_);
    /*life bar*/
    GameObject &lifeBar = scene_->add<Engine::LowRenderer::Entity>(sphere);
    lifeBar.entity->setTranslation({0.f, 1.f, 0.f});
    lifeBar.entity->setName("lifeBar");

    // ModelCreateArg lifeBarBackGroundArg{{0.f, 0.f, 0.f},
    //                                     {0.f, 0.f, 0.f},
    //                                     {1.f, 0.1f, 0.2f},
    //                                     &ressourceManager.get<Shader>("White"),
    //                                     {&ressourceManager.get<Material>("BlackMaterial")},
    //                                     &ressourceManager.get<Mesh>("Plane1"),
    //                                     "lifeBarBG"};
    // scene_->add<BillBoard>(lifeBar, lifeBarBackGroundArg);

    ModelCreateArg lifeBarInternalArg{{0.f, 0.f, 0.1f},
                                      {0.f, 0.f, 0.f},
                                      {1.f, 0.001f, 0.1f},
                                      &ressourceManager.get<Shader>("White"),
                                      {&ressourceManager.get<Material>("GreenMaterial")},
                                      &ressourceManager.get<Mesh>("Plane1"),
                                      "lifeBarBGIndicator"};
    GameObject& lifeBarINternal = scene_->add<BillBoard>(lifeBar, lifeBarInternalArg);

    sphere.addComponent<PhysicalObject>();
    sphere.getComponent<PhysicalObject>()->SetMass(10);
    sphere.addComponent<SphereCollider>();
    sphere.getComponent<SphereCollider>()->SetBounciness(0.5f);
    scene_->getGameObject("world/cube1").addComponent<OrientedBoxCollider>();

    lifeBarINternal.addComponent<BarIndicatorController<float>>(testLifePLayer, testLifePLayer);
}

void Demo::loadUI(t_RessourcesManager &ressourceManager)
{
    FontCreateArg fontarg {"./ressources/opensans.ttf", 40};
    Font * pfont = &ressourceManager.add<Font>("font1", fontarg);
    FontCreateArg fontarg2 {"./ressources/opensans.ttf", 25};
    Font * pfont2 = &ressourceManager.add<Font>("font2", fontarg2);
    Shader* buttonShader = &ressourceManager.add<Shader>("ButtonShader", "./ressources/shader/text.vs", "./ressources/shader/texture.fs");

    int tempX = gameEngine_.getWinSize().width / 2.0f;
    int tempY = gameEngine_.getWinSize().heigth / 2.0f;

    #pragma region Start
    ressourceManager.add<Button>("MenuStartButton", pfont, buttonShader, 
                                            tempX - 90, tempY - 200, 
                                            200.0f, 60.0f, SDL_Color{170, 80, 80, 0}, "New Game",
                                            E_GAME_STATE::STARTING).function = [&]()
    {
        gameEngine_.gameState = E_GAME_STATE::RUNNING;
        usingMouse = false;
    };

    ressourceManager.add<Button>("MenuLoadButton", pfont, buttonShader, 
                                            tempX - 95, tempY - 100, 
                                            220.0f, 60.0f, SDL_Color{170, 170, 80, 0}, "Load Game",
                                            E_GAME_STATE::STARTING).function = [&]()
    {
        
    };

    ressourceManager.add<Button>("MenuOptionButton", pfont, buttonShader, 
                                            tempX - 65, tempY, 
                                            150.0f, 60.0f, SDL_Color{80, 170, 170, 0}, "Options",
                                            E_GAME_STATE::STARTING).function = [&]()
    {
        gameEngine_.gameState = E_GAME_STATE::OPTION;
    };

    ressourceManager.add<Button>("MenuQuitButton", pfont, buttonShader,
                                            tempX - 35, tempY + 100, 
                                            150.0f, 60.0f, SDL_Color{80, 80, 170, 0}, "Quit",
                                            E_GAME_STATE::STARTING).function = [&]()
    {
        gameEngine_.gameState = E_GAME_STATE::EXIT;
    };
    
    #pragma endregion 

    #pragma region Pause

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
    
    #pragma endregion

    #pragma region Option

    ressourceManager.add<Title>("OptionForwardTitle",   pfont, buttonShader,
                                                    tempX - 155, tempY - 300, 
                                                    175.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Forward :",
                                                    E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionForwardButton",  pfont, buttonShader,
                                                    tempX + 50, tempY - 300, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(gameEngine_.inputManager_.keyboard.up)),
                                                    E_GAME_STATE::OPTION).function = [&]()
    {
        SDL_Scancode key = gameEngine_.inputManager_.waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            gameEngine_.inputManager_.keyboard.up = key;
            ressourceManager.get<Button>("OptionForwardButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionForwardButton").updateTexture();
            gameEngine_.inputManager_.resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionBackwardTitle",   pfont, buttonShader,
                                                    tempX - 185, tempY - 200, 
                                                    200.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Backward :",
                                                    E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionBackwardButton",  pfont, buttonShader,
                                                    tempX + 50, tempY - 200, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(gameEngine_.inputManager_.keyboard.down)),
                                                    E_GAME_STATE::OPTION).function = [&]()
    {
        SDL_Scancode key = gameEngine_.inputManager_.waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            gameEngine_.inputManager_.keyboard.down = key;
            ressourceManager.get<Button>("OptionBackwardButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionBackwardButton").updateTexture();
            gameEngine_.inputManager_.resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionLeftTitle",   pfont, buttonShader,
                                                    tempX - 75, tempY - 100, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Left :",
                                                    E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionLeftButton",  pfont, buttonShader,
                                                    tempX + 50, tempY - 100, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(gameEngine_.inputManager_.keyboard.left)),
                                                    E_GAME_STATE::OPTION).function = [&]()
    {
        SDL_Scancode key = gameEngine_.inputManager_.waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            gameEngine_.inputManager_.keyboard.left = key;
            ressourceManager.get<Button>("OptionLeftButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionLeftButton").updateTexture();
            gameEngine_.inputManager_.resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionRightTitle",   pfont, buttonShader,
                                                    tempX - 100, tempY, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Right :",
                                                    E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionRightButton",  pfont, buttonShader,
                                                    tempX + 50, tempY, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(gameEngine_.inputManager_.keyboard.right)),
                                                    E_GAME_STATE::OPTION).function = [&]()
    {
        SDL_Scancode key = gameEngine_.inputManager_.waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            gameEngine_.inputManager_.keyboard.right = key;
            ressourceManager.get<Button>("OptionRightButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionRightButton").updateTexture();
            gameEngine_.inputManager_.resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionJumpTitle",   pfont, buttonShader,
                                                    tempX - 105, tempY + 100, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Jump :",
                                                    E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionJumpButton",  pfont, buttonShader,
                                                    tempX + 50, tempY + 100, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(gameEngine_.inputManager_.keyboard.jump)),
                                                    E_GAME_STATE::OPTION).function = [&]()
    {
        SDL_Scancode key = gameEngine_.inputManager_.waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            gameEngine_.inputManager_.keyboard.jump = key;
            ressourceManager.get<Button>("OptionJumpButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionJumpButton").updateTexture();
            gameEngine_.inputManager_.resetKeyDown();
        }
    };

    #pragma endregion

    #pragma region Saves

    int i = -300;
    int j = -300;

    std::string shortSaveName;

    for (std::string& saves : gameEngine_.savePaths)
    {
        if (saves.size() < 23) // TODO: assert
            return;
        shortSaveName = saves.substr(19, saves.size() - 23);
        ressourceManager.add<Button>(   shortSaveName,  pfont2, buttonShader,
                                        tempX + i, tempY + j, 
                                        75.0f, 60.0f, SDL_Color{200, 200, 200, 0}, 
                                        shortSaveName, E_GAME_STATE::STARTSAVE).function = [&]()
        {
            gameEngine_.gameState = E_GAME_STATE::RUNNING;
            usingMouse = false;
            scene_.reset();
            scene_ = std::make_unique<Scene>();
            setupScene(*scene_, gameEngine_, saves.c_str());
            mainCamera = &scene_->getGameObject("world/MainCamera");
        };

        i += 150;

        if (i == 300)
        {
            j += 100;
            i = -300;
        }
    }

    #pragma endregion
}

void Demo::loadLights      (t_RessourcesManager& ressourceManager)
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

    sunLight = &scene_->add<DirectionnalLight>(scene_->getWorld(), lightArg2);

    GameObject& pl1 = scene_->add<PointLight>(scene_->getWorld(), lightArg5);

    scene_->add<Model>(pl1, lightArg);

    static_cast<DirectionnalLight*>(sunLight->entity.get())->enable(true);
    static_cast<PointLight*>(pl1.entity.get())->enable(true);
}

void Demo::updateControl(Engine::Core::InputSystem::Input& input)
{
    testLifePLayer -= 0.1;

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
        else if (gameEngine_.gameState == E_GAME_STATE::OPTION 
             ||  gameEngine_.gameState == E_GAME_STATE::LOADSAVE 
             ||  gameEngine_.gameState == E_GAME_STATE::STARTSAVE)
        {
            gameEngine_.gameState = E_GAME_STATE::STARTING;
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

    // static int exFrameWheelVal = input.mouse.wheel_scrolling;

    // if (input.mouse.wheel_scrolling != exFrameWheelVal)
    // {
    //     if(input.mouse.wheel_scrolling > exFrameWheelVal)
    //     {
    //         static_cast<Camera*>(mainCamera->entity.get())->setFovY(static_cast<Camera*>(mainCamera->entity.get())->getProjectionInfo().fovY + 5);
    //     }
    //     else
    //     {
    //         static_cast<Camera*>(mainCamera->entity.get())->setFovY(static_cast<Camera*>(mainCamera->entity.get())->getProjectionInfo().fovY - 5);
    //     }
        
    //     exFrameWheelVal = input.mouse.wheel_scrolling;
    // }

    // if (input.keyboard.isDown[SDL_SCANCODE_F1] && !flagF1IsDown)
    // {
    //     mouseForPlayer1 = !mouseForPlayer1;
    //     flagF1IsDown = true;
    // }
    // else
    // {
    //     flagF1IsDown = input.keyboard.isDown[SDL_SCANCODE_F1];
    // }    
}