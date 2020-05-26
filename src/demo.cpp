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
#include "GE/Ressources/Saves.hpp"
#include "Game/BarIndicatorController.hpp"
#include "Game/CircularEnemiesSpawner.hpp"
#include "Game/ParticuleGenerator.hpp"
#include "Game/MaxElementConteneur.hpp"

#include "GE/Physics/ColliderShape/SphereCollider.hpp"
#include "GE/Physics/ColliderShape/OrientedBoxCollider.hpp"
#include "GE/Physics/ColliderShape/CapsuleCollider.hpp"
#include "GE/Physics/ColliderShape/AABBCollider.hpp"

#include "../src/stb_image.h"

#ifndef DNEDITOR
#include "GE/LowRenderer/EditorTools/Editor.hpp"
using namespace Engine::LowRenderer::EditorTools;
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
using namespace Engine::Ressources::Save;
using namespace Engine::LowRenderer;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Parsers;
using namespace Engine::Core::System;
using namespace Engine::Core::DataStructure;
using namespace Engine::Core::InputSystem;

std::unique_ptr<Engine::Ressources::Scene> *Demo::_currentScene;

Demo::Demo(Engine::GE &gameEngine)
    : _gameEngine{gameEngine},
      _scene{},
      flagleftClicIsDown{false},
      flagF1IsDown{false},
      usingMouse{true},
      dirCamera{0.f, 0.f, -1.f}
{
    _scene = std::make_unique<Scene>();
    _scene->use();
    _gameEngine.ressourceManager_.use();

    loadRessources(_gameEngine.ressourceManager_);
    loadCamera();
    loadEntity(_gameEngine.ressourceManager_);
    loadGround(_gameEngine.ressourceManager_);
    // loadSkyBox(_gameEngine.ressourceManager_);
    loadLights(_gameEngine.ressourceManager_);
    loadUI(_gameEngine.ressourceManager_);
    loadATH(_gameEngine.ressourceManager_);
    loadEnemies(_gameEngine.ressourceManager_);
    // setupScene(*_scene, _gameEngine, "./ressources/saves/testtest.xml");
    // mainCamera = &_scene->getGameObject("world/MainCamera");
    loadReferential(_gameEngine.ressourceManager_);

    ScriptSystem::start();

    // setupScene(_scene, _gameEngine, "./ressources/saves/setup.xml");

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    UISystem::isActive = true;

    _currentScene = &_scene;
}

void Demo::update() noexcept
{
    SDL_ShowCursor(usingMouse);
    SDL_CaptureMouse(SDL_TRUE);
    SDL_SetRelativeMouseMode(usingMouse ? SDL_FALSE : SDL_TRUE);

    UISystem::update(_gameEngine);

    updateControl();

    if (_gameEngine.gameState == E_GAME_STATE::RUNNING)
    {
        _scene->update();
        ScriptSystem::update();
    }

#ifndef DNEDITOR
    Editor::update(*_scene);
#endif
}

void Demo::fixedUpdate() noexcept
{
    if (_gameEngine.gameState == E_GAME_STATE::RUNNING)
    {
        PhysicSystem::update();
        ScriptSystem::fixedUpdate();
    }
}

void Demo::display() const noexcept
{
    Size sizeWin = _gameEngine.getWinSize();

    if (_gameEngine.gameState == E_GAME_STATE::RUNNING || _gameEngine.gameState == E_GAME_STATE::PAUSE)
    {
        glViewport(0, 0, sizeWin.width, sizeWin.heigth);

        RenderingSystem::draw();
    }

    UISystem::draw(_gameEngine);

    glUseProgram(0);
}

void Demo::loadRessources(t_RessourcesManager &ressourceManager)
{
    ressourceManager.add<Shader>("ColorWithLight", "./ressources/shader/vProjectionNormal.vs", "./ressources/shader/fColorWithLight.fs", AMBIANTE_COLOR_ONLY | LIGHT_BLIN_PHONG);
    ressourceManager.add<Shader>("Color", "./ressources/shader/vCloud.vs", "./ressources/shader/fColorOnly.fs", AMBIANTE_COLOR_ONLY);
    ressourceManager.add<Shader>("TextureOnly", "./ressources/shader/vCloud.vs", "./ressources/shader/fTextureOnly.fs");
    ressourceManager.add<Shader>("LightAndTexture", "./ressources/shader/vTexture2.vs", "./ressources/shader/fTexture2.fs", LIGHT_BLIN_PHONG);

    MaterialAndTextureCreateArg matDefault;
    matDefault.name_ = "DefaultMaterial";
    matDefault.pathDiffuseTexture = nullptr;
    matDefault.flipTexture = false;
    ressourceManager.add<Material>("DefaultMaterial", matDefault);

    matDefault.name_ = "PinkMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{1.f, 0.f, 1.f, 1.f};
    ressourceManager.add<Material>(matDefault.name_, matDefault);

    matDefault.name_ = "RedMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{1.f, 0.f, 0.f, 1.f};
    ressourceManager.add<Material>(matDefault.name_, matDefault);

    matDefault.name_ = "GreenMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{0.f, 1.f, 0.f, 1.f};
    ressourceManager.add<Material>(matDefault.name_, matDefault);

    matDefault.name_ = "BlueMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{0.f, 0.f, 1.f, 1.f};
    ressourceManager.add<Material>(matDefault.name_, matDefault);

    matDefault.name_ = "BlackMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{0.f, 0.f, 0.f, 1.f};
    ressourceManager.add<Material>(matDefault.name_, matDefault);

    ressourceManager.add<Mesh>("Cube", Mesh::createCube(1));
    ressourceManager.add<Mesh>("Sphere", Mesh::createSphere(25, 25));
    ressourceManager.add<Mesh>("Plane", Mesh::createPlane());

    MaterialAndTextureCreateArg matGround;
    matGround.name_ = "Ground";
    matGround.comp_.shininess = 1.f;
    matGround.comp_.specular.rgbi = {1.f, 1.f, 1.f, 0.2f};
    matGround.pathDiffuseTexture = "./ressources/texture/ground.jpg";
    matGround.wrapType = E_WrapType::MIRRORED_REPEAT;

    ressourceManager.add<Material>("materialGround", matGround);

    MaterialAndTextureCreateArg matBulletHole;
    matBulletHole.name_ = "BulletHole";
    matBulletHole.pathDiffuseTexture = "./ressources/texture/bulletHole.png";

    ressourceManager.add<Material>("BulletHole", matBulletHole);
}

void Demo::loadCamera()
{
    CameraPerspectiveCreateArg camArg{{0.f, 0.f, 30.f}, {0.f, 0.f, 0.f}, _gameEngine.getWinSize().width / static_cast<float>(_gameEngine.getWinSize().heigth), 0.1f, 10000.0f, 45.0f, "MainCamera"};
    mainCamera = &_scene->add<Camera>(_scene->getWorld(), camArg);
    dynamic_cast<Camera *>(mainCamera)->use();
}

void Demo::loadEntity(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg cubeGameObject{"cube1",
                                       {{-0.7f, -5.f, 0.f},
                                        {0.f, 0.f, 45.f},
                                        {5.f, 1.f, 5.f}}};

    ModelCreateArg cube1arg{&ressourceManager.get<Shader>("ColorWithLight"),
                            {&ressourceManager.get<Material>("PinkMaterial")},
                            &ressourceManager.get<Mesh>("Cube"),
                            "ColorWithLight",
                            {"PinkMaterial"},
                            "Cube"};

    _scene->add<GameObject>(_scene->getWorld(), cubeGameObject).addComponent<Model>(cube1arg);
    _scene->getGameObject("world/cube1").addComponent<OrientedBoxCollider>();

    GameObjectCreateArg cube2GameObject{"cube2",
                                        {{-5.f, -10.f, 0.f},
                                         {0.f, 0.f, -45.f},
                                         {5.f, 1.f, 5.f}}};

    ModelCreateArg cube2arg{&ressourceManager.get<Shader>("ColorWithLight"),
                            {&ressourceManager.get<Material>("DefaultMaterial")},
                            &ressourceManager.get<Mesh>("Cube"),
                            "ColorWithLight",
                            {"DefaultMaterial"},
                            "Cube"};

    _scene->add<GameObject>(_scene->getWorld(), cube2GameObject).addComponent<Model>(cube2arg);
    _scene->getGameObject("world/cube2").addComponent<OrientedBoxCollider>();

    GameObjectCreateArg cube3GameObject{"cube3",
                                        {{0.f, -11.f, 0.f},
                                         {0.f, 0.f, 45.f},
                                         {5.f, 1.f, 5.f}}};

    ModelCreateArg cube3arg{&ressourceManager.get<Shader>("ColorWithLight"),
                            {&ressourceManager.get<Material>("DefaultMaterial")},
                            &ressourceManager.get<Mesh>("Cube"),
                            "ColorWithLight",
                            {"DefaultMaterial"},
                            "Cube"};

    _scene->add<GameObject>(_scene->getWorld(), cube3GameObject).addComponent<Model>(cube3arg);
    _scene->getGameObject("world/cube3").addComponent<OrientedBoxCollider>();

    GameObjectCreateArg playerGameObject{"Player",
                                         {{-2.f, 0.f, 0.f},
                                          {0.f, 0.f, 0.f},
                                          {1.0f, 1.0f, 1.0f}}};

    ModelCreateArg playerModel{&ressourceManager.get<Shader>("ColorWithLight"),
                               {&ressourceManager.get<Material>("PinkMaterial")},
                               &ressourceManager.get<Mesh>("Sphere"),
                               "ColorWithLight",
                               {"PinkMaterial"},
                               "Sphere"};

    GameObject &player = _scene->add<GameObject>(_scene->getWorld(), playerGameObject);
    player.addComponent<Model>(playerModel);

    GameObjectCreateArg ReticuleGameObject{"Z",
                                           {{0.f, 0.f, 0.f},
                                            {0.f, 0.f, 0.f},
                                            {0.2f, 0.2f, 0.2f}}};

    ModelCreateArg ReticuleModel{&ressourceManager.get<Shader>("ColorWithLight"),
                                 {&ressourceManager.get<Material>("BlueMaterial")},
                                 &ressourceManager.get<Mesh>("Sphere"),
                                 "ColorWithLight",
                                 {"RedMaterial"},
                                 "Sphere"};
    /*
    GameObject &ReticuleX = _scene->add<GameObject>(_scene->getWorld(), ReticuleGameObject);
    ReticuleX.addComponent<Model>(ReticuleModel); 

    ReticuleGameObject.name = "Y";
    ReticuleModel.pMaterials = {&ressourceManager.get<Material>("GreenMaterial")};

    GameObject &ReticuleY = _scene->add<GameObject>(_scene->getWorld(), ReticuleGameObject);
    ReticuleY.addComponent<Model>(ReticuleModel); 

    ReticuleGameObject.name = "X";
    ReticuleModel.pMaterials = {&ressourceManager.get<Material>("RedMaterial")};

    GameObject &ReticuleZ = _scene->add<GameObject>(_scene->getWorld(), ReticuleGameObject);
    ReticuleZ.addComponent<Model>(ReticuleModel);*/

    /*Add life bar on player*/
    GameObjectCreateArg lifeBarGameObject{"lifeBar",
                                          {{0.f, 2.f, 0.f},
                                           {0.f, 0.f, 0.f},
                                           {1.f, 0.3f, 0.1f}}};

    ModelCreateArg billBoardArg{&ressourceManager.get<Shader>("Color"),
                                {&ressourceManager.get<Material>("GreenMaterial")},
                                &ressourceManager.get<Mesh>("Sphere"),
                                "Color",
                                {"GreenMaterial"},
                                "Sphere"};

    player.addComponent<PlayerController>();
    player.addComponent<PhysicalObject>();
    player.getComponent<PhysicalObject>()->setMass(1);
    player.addComponent<SphereCollider>();
    player.getComponent<SphereCollider>()->setBounciness(0.f);
}

void Demo::loadSkyBox(t_RessourcesManager &ressourceManager)
{
    MaterialAndTextureCreateArg matSKB;
    matSKB.name_ = "SkyBox";
    matSKB.pathDiffuseTexture = "./ressources/texture/skb.bmp";
    matSKB.flipTexture = false;
    matSKB.filterType = E_FilterType::LINEAR;

    Shader &skyboxShader = ressourceManager.add<Shader>("skyboxShader", "./ressources/shader/vSkybox.vs", "./ressources/shader/fSkybox.fs", SKYBOX);
    Material &materialSKB = ressourceManager.add<Material>("materialSKB", matSKB);
    Mesh &SKBMesh = ressourceManager.add<Mesh>("SKBMesh", "./ressources/obj/skybox.obj");

    GameObjectCreateArg skyboxArgGameObject{"Skybox",
                                            {{0.f, 0.f, 0.f},
                                             {0.f, 0.f, 0.f},
                                             {10.f, 10.f, 10.f}}};

    ModelCreateArg skyboxArg{&skyboxShader,
                             {&materialSKB},
                             &SKBMesh,
                             "skyboxShader",
                             {"materialSKB"},
                             "SKBMesh",
                             true,
                             false};

    _scene->add<GameObject>(_scene->getWorld(), skyboxArgGameObject).addComponent<Model>(skyboxArg);
}

void Demo::loadGround(t_RessourcesManager &ressourceManager)
{
    //Mesh&     ground            = ressourceManager.add<Mesh>("ground" ,Mesh::createPlane(50));

    GameObjectCreateArg groundArgGameObject{"Ground",
                                            {{0.f, -20.f, 0.f},
                                             {0.f, 0.f, 0.f},
                                             {50.f, 1.f, 50.f}}};

    ModelCreateArg groundArg{&ressourceManager.get<Shader>("TextureOnly"),
                             {&ressourceManager.get<Material>("materialGround")},
                             &ressourceManager.get<Mesh>("Cube"),
                             "TextureOnly",
                             {"materialGround"},
                             "Cube",
                             true,
                             false};

    GameObject &ground = _scene->add<GameObject>(_scene->getWorld(), groundArgGameObject);
    ground.addComponent<Model>(groundArg);
    ground.setTag("Ground");
    _scene->getGameObject("world/Ground").addComponent<OrientedBoxCollider>();
}

void Demo::loadLights(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg lightSphereGameObjectArg{"DirectionnalLight",
                                                 {{0.f, 0.f, 0.f},
                                                  {0.f, 0.f, 0.f},
                                                  {1.f, 1.f, 1.f}}};

    ModelCreateArg lightSphereArg{&ressourceManager.get<Shader>("ColorWithLight"),
                                  {&ressourceManager.get<Material>("DefaultMaterial")},
                                  &ressourceManager.get<Mesh>("Sphere"),
                                  "ColorWithLight",
                                  {"DefaultMaterial"},
                                  "Sphere",
                                  true};

    DirectionnalLightCreateArg lightArg2{{0.f, 1.f, -1.f},
                                         {1.f, 1.f, 1.f, 0.1f},
                                         {1.f, 1.f, 1.f, 0.7f},
                                         {1.f, 1.f, 1.f, 1.f}};

    GameObjectCreateArg pointLightGameObjectArg{"PointLight",
                                                {{5.f, 10.f, -5.f},
                                                 {0.f, 0.f, 0.f},
                                                 {1.f, 1.f, 1.f}}};

    PointLightCreateArg lightArg5{{1.f, 1.f, 1.f, 0.f},
                                  {0.f, 1.f, 0.f, 0.7f},
                                  {1.f, 1.f, 1.f, 0.3f},
                                  0.f,
                                  0.05f,
                                  0.f};

    GameObjectCreateArg spotLightGameObjectArg{"SpotLight",
                                               {{5.f, 10.f, -5.f},
                                                {0.f, 0.f, 0.f},
                                                {1.f, 1.f, 1.f}}};

    SpotLightCreateArg lightArg6{{1.f, 1.f, 1.f, 0.f},
                                 {0.f, 1.f, 0.f, 0.7f},
                                 {1.f, 1.f, 1.f, 0.3f},
                                 0.f,
                                 0.05f,
                                 0.f,
                                 Vec3::down,
                                 20.f,
                                 0.5f};

    GameObject &pl = _scene->add<GameObject>(_scene->getWorld(), lightSphereGameObjectArg);
    pl.addComponent<Model>(lightSphereArg);
    pl.addComponent<DirectionnalLight>(lightArg2).enable(true);

    GameObject &pl1 = _scene->add<GameObject>(_scene->getWorld(), pointLightGameObjectArg);
    pl1.addComponent<Model>(lightSphereArg);
    pl1.addComponent<PointLight>(lightArg5).enable(true);

    GameObject &pl2 = _scene->add<GameObject>(_scene->getWorld(), spotLightGameObjectArg);
    pl2.addComponent<Model>(lightSphereArg);
    pl2.addComponent<SpotLight>(lightArg6).enable(true);
}

void Demo::loadReferential(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg refGO{"Right ref",
                              {{100.f, 0.f, 0.f},
                               {0.f, 0.f, 0.f},
                               {5.f, 5.f, 5.f}}};

    ModelCreateArg sphereModel{&ressourceManager.get<Shader>("ColorWithLight"),
                               {&ressourceManager.get<Material>("RedMaterial")},
                               &ressourceManager.get<Mesh>("Sphere"),
                               "ColorWithLight",
                               {"RedMaterial"},
                               "Sphere",
                               true};

    _scene->add<GameObject>(_scene->getWorld(), refGO).addComponent<Model>(sphereModel);

    refGO.name = "Forward ref";
    refGO.transformArg.position = {0.f, 0.f, 100.f};
    sphereModel.pMaterials = {&ressourceManager.get<Material>("BlueMaterial")};
    _scene->add<GameObject>(_scene->getWorld(), refGO).addComponent<Model>(sphereModel);

    refGO.name = "Up ref";
    refGO.transformArg.position = {0.f, 100.f, 0.f};
    sphereModel.pMaterials = {&ressourceManager.get<Material>("GreenMaterial")};
    _scene->add<GameObject>(_scene->getWorld(), refGO).addComponent<Model>(sphereModel);
}

void Demo::loadUI(t_RessourcesManager &ressourceManager)
{
    FontCreateArg fontarg{"./ressources/opensans.ttf", 40};
    Font *pfont = &ressourceManager.add<Font>("font1", fontarg);
    FontCreateArg fontarg2{"./ressources/opensans.ttf", 25};
    Font *pfont2 = &ressourceManager.add<Font>("font2", fontarg2);
    Shader *buttonShader = &ressourceManager.add<Shader>("ButtonShader", "./ressources/shader/text.vs", "./ressources/shader/texture.fs");

    int tempX = _gameEngine.getWinSize().width / 2.0f;
    int tempY = _gameEngine.getWinSize().heigth / 2.0f;

#pragma region Start
    ressourceManager.add<Button>("MenuStartButton", pfont, buttonShader,
                                 tempX - 90, tempY - 200,
                                 200.0f, 60.0f, SDL_Color{170, 80, 80, 0}, "New Game",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::RUNNING;
        usingMouse = false;
    };

    ressourceManager.add<Button>("MenuLoadButton", pfont, buttonShader,
                                 tempX - 95, tempY - 100,
                                 220.0f, 60.0f, SDL_Color{170, 170, 80, 0}, "Load Game",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::STARTSAVE;
    };

    ressourceManager.add<Button>("MenuOptionButton", pfont, buttonShader,
                                 tempX - 65, tempY,
                                 150.0f, 60.0f, SDL_Color{80, 170, 170, 0}, "Options",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::OPTION;
    };

    ressourceManager.add<Button>("MenuQuitButton", pfont, buttonShader,
                                 tempX - 35, tempY + 100,
                                 150.0f, 60.0f, SDL_Color{80, 80, 170, 0}, "Quit",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::EXIT;
    };

#pragma endregion

#pragma region Pause

    ressourceManager.add<Button>("PausePlayButton", pfont, buttonShader,
                                 tempX - 35, tempY - 100,
                                 150.0f, 60.0f, SDL_Color{170, 80, 170, 0}, "Play",
                                 E_GAME_STATE::PAUSE)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::RUNNING;
        usingMouse = false;
    };
    ressourceManager.add<Button>("PauseMenuButton", pfont, buttonShader,
                                 tempX - 45, tempY,
                                 150.0f, 60.0f, SDL_Color{80, 170, 80, 0}, "Menu",
                                 E_GAME_STATE::PAUSE)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::STARTING;
        usingMouse = true;
    };

#pragma endregion

#pragma region Option

    ressourceManager.add<Title>("OptionForwardTitle", pfont, buttonShader,
                                tempX - 155, tempY - 300,
                                175.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Forward :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionForwardButton", pfont, buttonShader,
                                 tempX + 50, tempY - 300,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.up)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.up = key;
            ressourceManager.get<Button>("OptionForwardButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionForwardButton").updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionBackwardTitle", pfont, buttonShader,
                                tempX - 185, tempY - 200,
                                200.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Backward :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionBackwardButton", pfont, buttonShader,
                                 tempX + 50, tempY - 200,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.down)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.down = key;
            ressourceManager.get<Button>("OptionBackwardButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionBackwardButton").updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionLeftTitle", pfont, buttonShader,
                                tempX - 75, tempY - 100,
                                150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Left :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionLeftButton", pfont, buttonShader,
                                 tempX + 50, tempY - 100,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.left)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.left = key;
            ressourceManager.get<Button>("OptionLeftButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionLeftButton").updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionRightTitle", pfont, buttonShader,
                                tempX - 100, tempY,
                                150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Right :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionRightButton", pfont, buttonShader,
                                 tempX + 50, tempY,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.right)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.right = key;
            ressourceManager.get<Button>("OptionRightButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionRightButton").updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionJumpTitle", pfont, buttonShader,
                                tempX - 105, tempY + 100,
                                150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Jump :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionJumpButton", pfont, buttonShader,
                                 tempX + 50, tempY + 100,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.jump)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.jump = key;
            ressourceManager.get<Button>("OptionJumpButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionJumpButton").updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Button>("Return", pfont, buttonShader,
                                 tempX - 50, tempY + 250,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Return",
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::STARTING;
    };

#pragma endregion

#pragma region Saves

    int i = -300;
    int j = -300;

    std::string shortSaveName;

    for (std::string &saves : _gameEngine.savePaths)
    {
        if (saves.size() < 23) // TODO: assert
            return;

        shortSaveName = saves.substr(19, saves.size() - 23);
        ressourceManager.add<Button>(shortSaveName, pfont2, buttonShader,
                                     tempX + i, tempY + j,
                                     75.0f, 60.0f, SDL_Color{200, 200, 200, 0},
                                     shortSaveName, E_GAME_STATE::STARTSAVE)
            .function = [&]() {
            _gameEngine.gameState = E_GAME_STATE::RUNNING;
            usingMouse = false;
            Light::resetLight();
            _scene.reset();
            _scene = std::make_unique<Scene>();
            setupScene(*_scene, _gameEngine, saves.c_str());
            mainCamera = &_scene->getGameObject("world/MainCamera");
        };

        i += 150;

        if (i == 300)
        {
            j += 100;
            i = -300;
        }
    }

    ressourceManager.add<Button>("ReturnSave", pfont, buttonShader,
                                 tempX - 50, tempY + 250,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Return",
                                 E_GAME_STATE::STARTSAVE)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::STARTING;
    };

#pragma endregion
}

void Demo::loadATH(t_RessourcesManager &ressourceManager)
{
    float halfWidth = _gameEngine.getWinSize().width / 2.f;
    float halfHeight = _gameEngine.getWinSize().heigth / 2.f;
    float crosshairSize = _gameEngine.getWinSize().heigth / 15;
    float halfcrosshairSize = crosshairSize * 0.5;

    Shader *imageShader = &ressourceManager.add<Shader>("ImageShader",
                                                        "./ressources/shader/text.vs",
                                                        "./ressources/shader/texture.fs");

    TextureCreateArg tcaCrosshair{
        "./ressources/texture/crossair.png",
        E_WrapType::CLAMP_TO_BORDER,
    };

    Texture &t_crosshair = ressourceManager.add<Texture>("crosshair", tcaCrosshair);

    Image &image = ressourceManager.add<Image>("CrosshairImage",
                                               t_crosshair.getID(),
                                               imageShader,
                                               halfWidth - halfcrosshairSize,
                                               halfHeight - halfcrosshairSize,
                                               crosshairSize,
                                               crosshairSize,
                                               E_GAME_STATE::RUNNING);
}

void Demo::loadEnemies(Engine::Ressources::t_RessourcesManager &ressourceManager)
{
    enemiesContener = &_scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"EnemiesContener"});

    GameObjectCreateArg Ennemy1GameObjectArg{"Ennemy"};

    ModelCreateArg modelArg{&ressourceManager.get<Shader>("ColorWithLight"),
                            {&ressourceManager.get<Material>("GreenMaterial")},
                            &ressourceManager.get<Mesh>("Sphere"),
                            "ColorWithLight",
                            {"GreenMaterial"},
                            "Sphere"};

    GameObject &enemy1 = _scene->add<GameObject>(_scene->getWorld(), Ennemy1GameObjectArg);

    enemy1.addComponent<Model>(modelArg);
    enemy1.addComponent<PhysicalObject>().setMass(1);
    enemy1.addComponent<SphereCollider>().setBounciness(0.4f);

    Save::createPrefab(enemy1, "enemy1");
    enemy1.destroy();
    enemiesContener->addComponent<CircularEnemiesSpawner>(EnemieInfo{{std::string("enemy1")}}, Vec3{0.f, 4.f, 0.f}, 2.f, 1.f, 0.f);

    //enemiesContener->addComponent<CircularEnemiesSpawner>(EnemieInfo{{modelArg}, {modelArg2}}, Vec3{0.f, 4.f, 0.f}, 2.f, 1.f, 0.f);

    ModelCreateArg modelArg3{&ressourceManager.get<Shader>("Color"),
                             {&ressourceManager.get<Material>("GreenMaterial")},
                             &ressourceManager.get<Mesh>("Plane")};

    ParticuleGenerator::ParticleSystemCreateArg particalArg;
    particalArg.modelCreateArg = modelArg3;
    particalArg.isBillBoard = false;
    particalArg.physicalObjectCreateArg.useGravity = false;
    particalArg.useScaledTime = true;
    particalArg.velocityEvolutionCoef = 0.5f;
    particalArg.spawnCountBySec = 10.f;
    particalArg.lifeDuration = 10.f;
    particalArg.physicalObjectCreateArg.mass = 1.f;
    particalArg.scale = {0.1, 0.1, 0.1};

    //GameObject& particleGO = _scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"ParticleContener", {{0.f, 10.f, 0.f}}});
    //particleGO.addComponent<ParticuleGenerator>(particalArg);
    //particleGO.addComponent<LifeDuration>(10.f);

    _scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"DecalContenor", {{0.f, 0.f, 0.f}}}).addComponent<MaxElementConteneur>(10);
}

void Demo::updateControl()
{
    /* Draw player referential
    float dist = 5.f;
    _scene->getGameObject("Z").setTranslation(_scene->getGameObject("Player").getPosition() + dist * _scene->getGameObject("Player").getVecForward());
    _scene->getGameObject("Y").setTranslation(_scene->getGameObject("Player").getPosition() + dist * _scene->getGameObject("Player").getVecUp());
    _scene->getGameObject("X").setTranslation(_scene->getGameObject("Player").getPosition() + dist * _scene->getGameObject("Player").getVecRight());
    */

    if (Input::keyboard.getKeyState(SDL_SCANCODE_ESCAPE) == E_KEY_STATE::TOUCHED)
    {
        if (_gameEngine.gameState == E_GAME_STATE::RUNNING)
        {
            _gameEngine.gameState = E_GAME_STATE::PAUSE;
            usingMouse = true;
        }
        else if (_gameEngine.gameState == E_GAME_STATE::PAUSE)
        {
            _gameEngine.gameState = E_GAME_STATE::RUNNING;
            usingMouse = false;
        }
        else if (_gameEngine.gameState == E_GAME_STATE::OPTION || _gameEngine.gameState == E_GAME_STATE::LOADSAVE || _gameEngine.gameState == E_GAME_STATE::STARTSAVE)
        {
            _gameEngine.gameState = E_GAME_STATE::STARTING;
        }
        else if (_gameEngine.gameState == E_GAME_STATE::STARTING)
        {
            _gameEngine.gameState = E_GAME_STATE::EXIT;
        }
    }

    if (Input::keyboard.getKeyState(SDL_SCANCODE_F6) == E_KEY_STATE::TOUCHED)
    {
        saveScene(*_scene, _gameEngine, "./ressources/saves/testtest.xml");
    }
}
