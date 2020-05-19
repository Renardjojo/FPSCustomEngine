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
#include "Game/CircularEnemiesSpawner.hpp"

#include "GE/Physics/ColliderShape/SphereCollider.hpp"
#include "GE/Physics/ColliderShape/OrientedBoxCollider.hpp"
#include "GE/Physics/ColliderShape/CapsuleCollider.hpp"
#include "GE/Physics/ColliderShape/AABBCollider.hpp"

#include "../src/stb_image.h"

#ifndef DNEDITOR
#include "GE/LowRenderer/GraphicsDeviceInterface/SceneGraphWindow.hpp"
using namespace Engine::LowRenderer::GraphicsDeviceInterface;
#endif

#include "GE/Core/System/RenderingSystem.hpp"
#include "GE/Core/System/ScriptSystem.hpp"
#include "Game/PlayerController.hpp"
#include "Game/EnnemyController.hpp"

#include <SDL2/SDL_mouse.h>
#include "glad/glad.h"

using namespace Game;
using namespace Engine;
using namespace Engine::Physics;
using namespace Engine::Physics::ColliderShape;
using namespace Engine::Ressources;
using namespace Engine::LowRenderer;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Parsers;
using namespace Engine::Core::System;
using namespace Engine::Core::DataStructure;
using namespace Engine::Core::InputSystem;


Demo::Demo(Engine::GE& gameEngine)
    :   gameEngine_         (gameEngine),
        scene_              (),
        flagleftClicIsDown  (false),
        flagF1IsDown        (false),
        usingMouse          (true),
        dirCamera          {0.f, 0.f, -1.f}
{

    scene_ = std::make_unique<Scene>();

    // TimeSystem::setTimeScale(0.3f);

    loadRessources(gameEngine_.ressourceManager_);
    
    loadCamera(gameEngine_.ressourceManager_);

    loadEntity(gameEngine_.ressourceManager_);
    // loadSkateBoard(gameEngine_.ressourceManager_);
    loadGround(gameEngine_.ressourceManager_);
    // loadSkyBox(gameEngine_.ressourceManager_);
    loadLights(gameEngine_.ressourceManager_);
    
    loadUI(gameEngine_.ressourceManager_);

    loadEnemies(gameEngine_.ressourceManager_);

    ScriptSystem::start();

    // setupScene(scene_, gameEngine_, "./ressources/saves/setup.xml");

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
    updateControl();

    if (gameEngine_.gameState == E_GAME_STATE::RUNNING)
    {
        scene_->update();
        ScriptSystem::update();
    }

#ifndef DNEDITOR
    updateEditor();
#endif
}

void Demo::fixedUpdate    () noexcept
{
    if (gameEngine_.gameState == E_GAME_STATE::RUNNING)
    {
        PhysicSystem::update();
        ScriptSystem::fixedUpdate();
    }
}

void Demo::display    () const noexcept
{
    Size sizeWin = gameEngine_.getWinSize();

    if (gameEngine_.gameState == E_GAME_STATE::RUNNING || gameEngine_.gameState == E_GAME_STATE::PAUSE)
    {
        glViewport(0, 0, sizeWin.width, sizeWin.heigth);
        dynamic_cast<Camera*>(mainCamera)->use();
        
        RenderingSystem::draw();
    }
    
    UISystem::draw(gameEngine_);
    
    glUseProgram(0);
}

void Demo::loadRessources(t_RessourcesManager &ressourceManager)
{
    ressourceManager.add<Shader>("ColorWithLight", "./ressources/shader/vProjectionNormal.vs", "./ressources/shader/fColorWithLight.fs", AMBIANTE_COLOR_ONLY | LIGHT_BLIN_PHONG);
    ressourceManager.add<Shader>("TextureOnly", "./ressources/shader/vCloud.vs", "./ressources/shader/fTextureOnly.fs");

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

    ressourceManager.add<Mesh>("Cube", Mesh::createCube(1));
    ressourceManager.add<Mesh>("Sphere", Mesh::createSphere(25, 25));
    ressourceManager.add<Mesh>("Plane1", Mesh::createPlane());
        
    MaterialAndTextureCreateArg matGround;
    matGround.name_                = "Ground";
    matGround.comp_.shininess      = 1.f;
    matGround.comp_.specular.rgbi  = {1.f, 1.f, 1.f, 0.2f};
    matGround.pathDiffuseTexture   = "./ressources/texture/ground.jpg";
    matGround.wrapType             = E_WrapType::MIRRORED_REPEAT;

    ressourceManager.add<Material>("materialGround" ,matGround);
}

void Demo::loadCamera(t_RessourcesManager &ressourceManager)
{
    CameraPerspectiveCreateArg camArg {{0.f, 0.f, 30.f}, {0.f, 0.f, 0.f}, gameEngine_.getWinSize().width / static_cast<float>(gameEngine_.getWinSize().heigth), 0.1f, 10000.0f, 45.0f, "MainCamera"};
    mainCamera = &scene_->add<Camera>(scene_->getWorld(), camArg);
    dynamic_cast<Camera *>(mainCamera)->use();
}

void Demo::loadEntity(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg cubeGameObject {"cube1",
                                        {{-0.7f, -5.f, 0.f}, 
                                        {0.f, 0.f, 45.f}, 
                                        {5.f, 1.f, 5.f}}};

    ModelCreateArg cube1arg     {&ressourceManager.get<Shader>("ColorWithLight"), 
                                {&ressourceManager.get<Material>("DefaultMaterial")}, 
                                &ressourceManager.get<Mesh>("Cube")};

    scene_->add<GameObject>(scene_->getWorld(), cubeGameObject).addComponent<Model>(cube1arg);
    scene_->getGameObject("world/cube1").addComponent<OrientedBoxCollider>();

    GameObjectCreateArg cube2GameObject {"cube2",
                                        {{-5.f, -10.f, 0.f}, 
                                        {0.f, 0.f, -45.f}, 
                                        {5.f, 1.f, 5.f}}};

    ModelCreateArg cube2arg     {&ressourceManager.get<Shader>("ColorWithLight"), 
                                {&ressourceManager.get<Material>("DefaultMaterial")}, 
                                &ressourceManager.get<Mesh>("Cube")};

    scene_->add<GameObject>(scene_->getWorld(), cube2GameObject).addComponent<Model>(cube2arg);
    scene_->getGameObject("world/cube2").addComponent<OrientedBoxCollider>();

    GameObjectCreateArg cube3GameObject {"cube3",
                                        {{0.f, -11.f, 0.f}, 
                                        {0.f, 0.f, 45.f}, 
                                        {5.f, 1.f, 5.f}}};

    ModelCreateArg cube3arg     {&ressourceManager.get<Shader>("ColorWithLight"), 
                                {&ressourceManager.get<Material>("DefaultMaterial")}, 
                                &ressourceManager.get<Mesh>("Cube")};

    scene_->add<GameObject>(scene_->getWorld(), cube3GameObject).addComponent<Model>(cube3arg);
    scene_->getGameObject("world/cube3").addComponent<OrientedBoxCollider>();

    GameObjectCreateArg playerGameObject    {"Player",
                                            {{0.f, 0.f, 0.f},
                                            {0.f, 0.f, 0.f},
                                            {1.0f, 1.0f, 1.0f}}};

    ModelCreateArg playerModel{&ressourceManager.get<Shader>("ColorWithLight"),
                          {&ressourceManager.get<Material>("PinkMaterial")},
                          &ressourceManager.get<Mesh>("Sphere")};

    GameObject& player = scene_->add<GameObject>(scene_->getWorld(), playerGameObject);
    player.addComponent<Model>(playerModel);

    /*Add life bar on player*/
    GameObjectCreateArg lifeBarGameObject {"lifeBar",
                                         {{0.f, 2.f, 0.f},
                                         {0.f, 0.f, 0.f},
                                         {1.f, 0.3f, 0.1f}}};

    ModelCreateArg billBoardArg {&ressourceManager.get<Shader>("ColorWithLight"),
                                {&ressourceManager.get<Material>("GreenMaterial")},
                                &ressourceManager.get<Mesh>("Cube")};

    scene_->add<GameObject>(player, lifeBarGameObject).addComponent<BillBoard>(billBoardArg);

    player.addComponent<PlayerController>();
    player.addComponent<PhysicalObject>();
    player.getComponent<PhysicalObject>()->SetMass(1);
    player.addComponent<SphereCollider>();
    player.getComponent<SphereCollider>()->SetBounciness(0.4f);
}

void Demo::loadSkyBox             (t_RessourcesManager &ressourceManager)
{
    MaterialAndTextureCreateArg matSKB;
    matSKB.name_                = "SkyBox";
    matSKB.pathDiffuseTexture   = "./ressources/texture/skb.bmp";
    matSKB.flipTexture          = false;
    matSKB.filterType           = E_FilterType::LINEAR;

    Material&   materialSKB = ressourceManager.add<Material>("materialSKB", matSKB);
    Mesh&       SKBMesh     = ressourceManager.add<Mesh>("SKBMesh", "./ressources/obj/skybox.obj");
    Shader&     skyboxShader= ressourceManager.add<Shader>("skyboxShader", "./ressources/shader/vSkybox.vs", "./ressources/shader/fSkybox.fs", SKYBOX);

    GameObjectCreateArg skyboxArgGameObject {"Skybox",
                                            {{0.f, 0.f, 0.f}, 
                                            {0.f, 0.f, 0.f}, 
                                            {10.f, 10.f, 10.f}}};

    ModelCreateArg skyboxArg    {&skyboxShader,
                                {&materialSKB},
                                &SKBMesh,
                                true, false};

    scene_->add<GameObject>(scene_->getWorld(), skyboxArgGameObject).addComponent<Model>(skyboxArg);
}

void Demo::loadGround             (t_RessourcesManager &ressourceManager)
{
    //Mesh&     ground            = ressourceManager.add<Mesh>("ground" ,Mesh::createPlane(50));

    GameObjectCreateArg groundArgGameObject {"Ground",
                                            {{0.f, -20.f, 0.f}, 
                                            {0.f, 0.f, 0.f}, 
                                            {50.f, 0.1f, 50.f}}};

    ModelCreateArg groundArg    {&ressourceManager.get<Shader>("TextureOnly"), 
                                {&ressourceManager.get<Material>("materialGround")},
                                &ressourceManager.get<Mesh>("Cube"),
                                true, false};

    scene_->add<GameObject>(scene_->getWorld(), groundArgGameObject).addComponent<Model>(groundArg);
    scene_->getGameObject("world/Ground").addComponent<OrientedBoxCollider>();
}

void Demo::loadSkateBoard         (t_RessourcesManager &ressourceManager)
{
    //ressourceManager.add<Shader>("LightAndTexture", "./resources/shader/vTexture2.vs", "./resources/shader/fTexture2.fs", LIGHT_BLIN_PHONG);

    //Load Character
    Attrib                      attrib;
    std::vector<Shape>          shape;
    std::vector<MaterialAttrib> materials;
    
    loadObjWithMTL("./ressources/obj/11703_skateboard_v1_L3.obj", &attrib, &shape, &materials);
    
    Mesh& meshMan = ressourceManager.add<Mesh>("SkateBoard", attrib, shape);
    std::vector<Material*> pMaterial;
    pMaterial.reserve(materials.size());

    for (size_t i = 0; i < materials.size(); i++)
    {
        Material& material = ressourceManager.add<Material>(std::string("SkateBoard") + std::to_string(i), materials[i]);
        pMaterial.push_back(&material);               
    }

    GameObjectCreateArg manArgGameObject    {"Man",
                                            {{0.f, -1.f, 0.f},
                                            {-M_PI_2, 0.f, 0.f}, 
                                            {0.05f, 0.05f, 0.05f}}};

    ModelCreateArg manArg     { &ressourceManager.get<Shader>("TextureOnly"),
                                pMaterial,
                                &meshMan,
                                true};

    scene_->add<GameObject>(scene_->getGameObject("world/Player"), manArgGameObject).addComponent<Model>(manArg);
}

void Demo::loadLights      (t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg lightSphereGameObjectArg    {"LightSphere",
                                                    {{0.f, 0.f, 0.f},
                                                    {0.f, 0.f, 0.f},
                                                    {1.f, 1.f, 1.f}}};

    ModelCreateArg lightSphereArg       {&ressourceManager.get<Shader>("ColorWithLight"),
                                        {&ressourceManager.get<Material>("DefaultMaterial")},
                                        &ressourceManager.get<Mesh>("Sphere"),
                                        true};

    DirectionnalLightCreateArg lightArg2 {   {0.f, 1.f, -1.f},
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

    scene_->add<GameObject>(pl1, lightSphereGameObjectArg).addComponent<Model>(lightSphereArg);

    dynamic_cast<DirectionnalLight*>(sunLight)->enable(true);
    //static_cast<PointLight*>(pl1.entity.get())->enable(true);
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
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.up)),
                                                    E_GAME_STATE::OPTION).function = [&]()
    {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.up = key;
            ressourceManager.get<Button>("OptionForwardButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionForwardButton").updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionBackwardTitle",   pfont, buttonShader,
                                                    tempX - 185, tempY - 200, 
                                                    200.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Backward :",
                                                    E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionBackwardButton",  pfont, buttonShader,
                                                    tempX + 50, tempY - 200, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.down)),
                                                    E_GAME_STATE::OPTION).function = [&]()
    {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.down = key;
            ressourceManager.get<Button>("OptionBackwardButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionBackwardButton").updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionLeftTitle",   pfont, buttonShader,
                                                    tempX - 75, tempY - 100, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Left :",
                                                    E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionLeftButton",  pfont, buttonShader,
                                                    tempX + 50, tempY - 100, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.left)),
                                                    E_GAME_STATE::OPTION).function = [&]()
    {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.left = key;
            ressourceManager.get<Button>("OptionLeftButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionLeftButton").updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionRightTitle",   pfont, buttonShader,
                                                    tempX - 100, tempY, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Right :",
                                                    E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionRightButton",  pfont, buttonShader,
                                                    tempX + 50, tempY, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.right)),
                                                    E_GAME_STATE::OPTION).function = [&]()
    {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.right = key;
            ressourceManager.get<Button>("OptionRightButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionRightButton").updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionJumpTitle",   pfont, buttonShader,
                                                    tempX - 105, tempY + 100, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Jump :",
                                                    E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionJumpButton",  pfont, buttonShader,
                                                    tempX + 50, tempY + 100, 
                                                    150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.jump)),
                                                    E_GAME_STATE::OPTION).function = [&]()
    {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.jump = key;
            ressourceManager.get<Button>("OptionJumpButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionJumpButton").updateTexture();
            Input::resetKeyDown();
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

void Demo::loadEnemies (Engine::Ressources::t_RessourcesManager& ressourceManager)
{
    enemiesContener = &scene_->add<GameObject>(scene_->getWorld(), GameObjectCreateArg{"EnemiesContener"});

    GameObjectCreateArg Ennemy1GameObjectArg    {"Ennemy"};

    ModelCreateArg modelArg{&ressourceManager.get<Shader>("ColorWithLight"),
                          {&ressourceManager.get<Material>("GreenMaterial")},
                          &ressourceManager.get<Mesh>("Sphere")};

    GameObjectCreateArg Ennemy2GameObjectArg    {"Ennemy2"};

    ModelCreateArg modelArg2{&ressourceManager.get<Shader>("ColorWithLight"),
                            {&ressourceManager.get<Material>("PinkMaterial")},
                            &ressourceManager.get<Mesh>("Cube")};

    enemiesContener->addComponent<CircularEnemiesSpawner>(EnemieInfo{{modelArg}, {modelArg2}}, Vec3{0.f, 4.f, 0.f}, 2.f, 0.5f, 0.f);
}

void Demo::updateControl()
{
    testLifePLayer -= 0.1;

    if (Input::keyboard.onePressed(SDL_SCANCODE_ESCAPE) == 1)
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

    if (Input::keyboard.isDown[SDL_SCANCODE_F1] && !flagF1IsDown)
    {
        usingMouse = !usingMouse;
#ifndef DNEDITOR
        displaySceneGraphWindows = !displaySceneGraphWindows;
#endif
        flagF1IsDown = true;
    }
    else
    {
        flagF1IsDown = Input::keyboard.isDown[SDL_SCANCODE_F1];
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

    // if (Input::keyboard.isDown[SDL_SCANCODE_SPACE])
    // {
    //     TimeSystem::setTimeScale(0.f);
    // }
    // else
    // {
    //     TimeSystem::setTimeScale(0.5f);
    // }

    // static int exFrameWheelVal = Input::mouse.wheel_scrolling;

    // if (Input::mouse.wheel_scrolling != exFrameWheelVal)
    // {
    //     if(Input::mouse.wheel_scrolling > exFrameWheelVal)
    //     {
    //         static_cast<Camera*>(mainCamera->entity.get())->setFovY(static_cast<Camera*>(mainCamera->entity.get())->getProjectionInfo().fovY + 5);
    //     }
    //     else
    //     {
    //         static_cast<Camera*>(mainCamera->entity.get())->setFovY(static_cast<Camera*>(mainCamera->entity.get())->getProjectionInfo().fovY - 5);
    //     }
        
    //     exFrameWheelVal = Input::mouse.wheel_scrolling;
    // }

    // if (Input::keyboard.isDown[SDL_SCANCODE_F1] && !flagF1IsDown)
    // {
    //     mouseForPlayer1 = !mouseForPlayer1;
    //     flagF1IsDown = true;
    // }
    // else
    // {
    //     flagF1IsDown = Input::keyboard.isDown[SDL_SCANCODE_F1];
    // }    
}

#ifndef DNEDITOR

void Demo::updateEditor()
{
    if (displaySceneGraphWindows)
    {
        SceneGraphWindow::update(*scene_);
    }
}

#endif