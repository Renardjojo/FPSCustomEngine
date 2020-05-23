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

std::unique_ptr<Engine::Ressources::Scene> *Demo::currentScene_;

Demo::Demo(Engine::GE& gameEngine)
    :   gameEngine_         (gameEngine),
        scene_              (),
        flagleftClicIsDown  (false),
        flagF1IsDown        (false),
        usingMouse          (true),
        dirCamera          {0.f, 0.f, -1.f}
{

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

    scene_ = std::make_unique<Scene>();
    scene_->use();

    // TimeSystem::setTimeScale(0.3f);

    loadRessources(gameEngine_.ressourceManager_);

    loadCamera();

    loadEntity(gameEngine_.ressourceManager_);
    loadGround(gameEngine_.ressourceManager_);
    // loadSkyBox(gameEngine_.ressourceManager_);
    loadLights(gameEngine_.ressourceManager_);

    loadUI(gameEngine_.ressourceManager_);

    loadATH(gameEngine_.ressourceManager_);

    loadEnemies(gameEngine_.ressourceManager_);

    // setupScene(*scene_, gameEngine_, "./ressources/saves/testtest.xml");
    // mainCamera = &scene_->getGameObject("world/MainCamera");
    loadReferential(gameEngine_.ressourceManager_);

    ScriptSystem::start();

    // setupScene(scene_, gameEngine_, "./ressources/saves/setup.xml");

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    UISystem::isActive = true;
    
    currentScene_ = &scene_;
}

void Demo::update() noexcept
{
    UISystem::update(gameEngine_);
    updateControl();

    if (gameEngine_.gameState == E_GAME_STATE::RUNNING)
    {
        scene_->update();
        ScriptSystem::update();
    }

#ifndef DNEDITOR
    Editor::update(*scene_);
#endif
}

void Demo::fixedUpdate() noexcept
{
    if (gameEngine_.gameState == E_GAME_STATE::RUNNING)
    {
        PhysicSystem::update();
        ScriptSystem::fixedUpdate();
    }
}

void Demo::display() const noexcept
{
    Size sizeWin = gameEngine_.getWinSize();

    if (gameEngine_.gameState == E_GAME_STATE::RUNNING || gameEngine_.gameState == E_GAME_STATE::PAUSE)
    {
        glViewport(0, 0, sizeWin.width, sizeWin.heigth);

        RenderingSystem::draw();
    }

    UISystem::draw(gameEngine_);

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

    ressourceManager.add<Material>("materialGround" ,matGround);

    MaterialAndTextureCreateArg matBulletHole;
    matBulletHole.name_                = "BulletHole";
    matBulletHole.pathDiffuseTexture   = "./ressources/texture/bulletHole.png";

    ressourceManager.add<Material>("BulletHole" ,matBulletHole);
}

void Demo::loadCamera()
{
    CameraPerspectiveCreateArg camArg{{0.f, 0.f, 30.f}, {0.f, 0.f, 0.f}, gameEngine_.getWinSize().width / static_cast<float>(gameEngine_.getWinSize().heigth), 0.1f, 10000.0f, 45.0f, "MainCamera"};
    mainCamera = &scene_->add<Camera>(scene_->getWorld(), camArg);
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

    scene_->add<GameObject>(scene_->getWorld(), cubeGameObject).addComponent<Model>(cube1arg);
    scene_->getGameObject("world/cube1").addComponent<OrientedBoxCollider>();

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

    scene_->add<GameObject>(scene_->getWorld(), cube2GameObject).addComponent<Model>(cube2arg);
    scene_->getGameObject("world/cube2").addComponent<OrientedBoxCollider>();

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

    scene_->add<GameObject>(scene_->getWorld(), cube3GameObject).addComponent<Model>(cube3arg);
    scene_->getGameObject("world/cube3").addComponent<OrientedBoxCollider>();

    GameObjectCreateArg playerGameObject{"Player",
                                         {{0.f, 0.f, 0.f},
                                          {0.f, 0.f, 0.f},
                                          {1.0f, 1.0f, 1.0f}}};

    ModelCreateArg playerModel{&ressourceManager.get<Shader>("ColorWithLight"),
                               {&ressourceManager.get<Material>("PinkMaterial")},
                               &ressourceManager.get<Mesh>("Sphere"),
                               "ColorWithLight",
                               {"PinkMaterial"},
                               "Sphere"};

    GameObject &player = scene_->add<GameObject>(scene_->getWorld(), playerGameObject);
    player.addComponent<Model>(playerModel);

    /*Add life bar on player*/
    GameObjectCreateArg lifeBarGameObject{"lifeBar",
                                          {{0.f, 2.f, 0.f},
                                           {0.f, 0.f, 0.f},
                                           {1.f, 0.3f, 0.1f}}};

    ModelCreateArg billBoardArg{&ressourceManager.get<Shader>("Color"),
                                {&ressourceManager.get<Material>("GreenMaterial")},
                                &ressourceManager.get<Mesh>("Cube"),
                                "Color",
                                {"GreenMaterial"},
                                "Cube"};

    GameObjectCreateArg rayGameObject{"Reticule",
                                      {{0.f, 0.f, 10.f},
                                       {0.f, 0.f, 0.f},
                                       {0.2f, 0.2f, 0.2f}}};

    ModelCreateArg rayArg   {&ressourceManager.get<Shader>("Color"),
                            {&ressourceManager.get<Material>("PinkMaterial")},
                            &ressourceManager.get<Mesh>("Sphere"),
                            "Color",
                            {"PinkMaterial"},
                            "Sphere"};

    scene_->add<GameObject>(player, rayGameObject).addComponent<Model>(rayArg);

    player.addComponent<PlayerController>(ressourceManager);
    player.addComponent<PhysicalObject>();
    player.getComponent<PhysicalObject>()->setMass(1);
    player.addComponent<SphereCollider>();
    player.getComponent<SphereCollider>()->setBounciness(0.4f);
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

    scene_->add<GameObject>(scene_->getWorld(), skyboxArgGameObject).addComponent<Model>(skyboxArg);
}

void Demo::loadGround(t_RessourcesManager &ressourceManager)
{
    //Mesh&     ground            = ressourceManager.add<Mesh>("ground" ,Mesh::createPlane(50));

    GameObjectCreateArg groundArgGameObject{"Ground",
                                            {{0.f, -20.f, 0.f},
                                             {0.f, 0.f, 0.f},
                                             {50.f, 0.1f, 50.f}}};

    ModelCreateArg groundArg{&ressourceManager.get<Shader>("TextureOnly"),
                             {&ressourceManager.get<Material>("materialGround")},
                             &ressourceManager.get<Mesh>("Cube"),
                             "TextureOnly",
                             {"materialGround"},
                             "Cube",
                             true,
                             false};

    scene_->add<GameObject>(scene_->getWorld(), groundArgGameObject).addComponent<Model>(groundArg);
    scene_->getGameObject("world/Ground").addComponent<OrientedBoxCollider>();
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

    GameObject &pl = scene_->add<GameObject>(scene_->getWorld(), lightSphereGameObjectArg);
    pl.addComponent<Model>(lightSphereArg);
    pl.addComponent<DirectionnalLight>(lightArg2).enable(true);

    GameObject &pl1 = scene_->add<GameObject>(scene_->getWorld(), pointLightGameObjectArg);
    pl1.addComponent<Model>(lightSphereArg);
    pl1.addComponent<PointLight>(lightArg5).enable(true);

    GameObject &pl2 = scene_->add<GameObject>(scene_->getWorld(), spotLightGameObjectArg);
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

    scene_->add<GameObject>(scene_->getWorld(), refGO).addComponent<Model>(sphereModel);

    refGO.name = "Forward ref";
    refGO.transformArg.position = {0.f, 0.f, 100.f};
    sphereModel.pMaterials = {&ressourceManager.get<Material>("BlueMaterial")};
    scene_->add<GameObject>(scene_->getWorld(), refGO).addComponent<Model>(sphereModel);

    refGO.name = "Up ref";
    refGO.transformArg.position = {0.f, 100.f, 0.f};
    sphereModel.pMaterials = {&ressourceManager.get<Material>("GreenMaterial")};
    scene_->add<GameObject>(scene_->getWorld(), refGO).addComponent<Model>(sphereModel);
}

void Demo::loadUI(t_RessourcesManager &ressourceManager)
{
    FontCreateArg fontarg{"./ressources/opensans.ttf", 40};
    Font *pfont = &ressourceManager.add<Font>("font1", fontarg);
    FontCreateArg fontarg2{"./ressources/opensans.ttf", 25};
    Font *pfont2 = &ressourceManager.add<Font>("font2", fontarg2);
    Shader *buttonShader = &ressourceManager.add<Shader>("ButtonShader", "./ressources/shader/text.vs", "./ressources/shader/texture.fs");

    int tempX = gameEngine_.getWinSize().width / 2.0f;
    int tempY = gameEngine_.getWinSize().heigth / 2.0f;

#pragma region Start
    ressourceManager.add<Button>("MenuStartButton", pfont, buttonShader,
                                 tempX - 90, tempY - 200,
                                 200.0f, 60.0f, SDL_Color{170, 80, 80, 0}, "New Game",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        gameEngine_.gameState = E_GAME_STATE::RUNNING;
        usingMouse = false;

        SDL_ShowCursor(false);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    };

    ressourceManager.add<Button>("MenuLoadButton", pfont, buttonShader,
                                 tempX - 95, tempY - 100,
                                 220.0f, 60.0f, SDL_Color{170, 170, 80, 0}, "Load Game",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        gameEngine_.gameState = E_GAME_STATE::STARTSAVE;
    };

    ressourceManager.add<Button>("MenuOptionButton", pfont, buttonShader,
                                 tempX - 65, tempY,
                                 150.0f, 60.0f, SDL_Color{80, 170, 170, 0}, "Options",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        gameEngine_.gameState = E_GAME_STATE::OPTION;
    };

    ressourceManager.add<Button>("MenuQuitButton", pfont, buttonShader,
                                 tempX - 35, tempY + 100,
                                 150.0f, 60.0f, SDL_Color{80, 80, 170, 0}, "Quit",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        gameEngine_.gameState = E_GAME_STATE::EXIT;
    };

#pragma endregion

#pragma region Pause

    ressourceManager.add<Button>("PausePlayButton", pfont, buttonShader,
                                 tempX - 35, tempY - 100,
                                 150.0f, 60.0f, SDL_Color{170, 80, 170, 0}, "Play",
                                 E_GAME_STATE::PAUSE)
        .function = [&]() {
        gameEngine_.gameState = E_GAME_STATE::RUNNING;
        usingMouse = false;

        SDL_ShowCursor(false);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    };
    ressourceManager.add<Button>("PauseMenuButton", pfont, buttonShader,
                                 tempX - 45, tempY,
                                 150.0f, 60.0f, SDL_Color{80, 170, 80, 0}, "Menu",
                                 E_GAME_STATE::PAUSE)
        .function = [&]() {
        gameEngine_.gameState = E_GAME_STATE::STARTING;
        usingMouse = true;

        SDL_ShowCursor(true);
        SDL_SetRelativeMouseMode(SDL_FALSE);
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
        gameEngine_.gameState = E_GAME_STATE::STARTING;
    };

#pragma endregion

#pragma region Saves

    int i = -300;
    int j = -300;

    std::string shortSaveName;

    for (std::string &saves : gameEngine_.savePaths)
    {
        if (saves.size() < 23) // TODO: assert
            return;

        shortSaveName = saves.substr(19, saves.size() - 23);
        ressourceManager.add<Button>(shortSaveName, pfont2, buttonShader,
                                     tempX + i, tempY + j,
                                     75.0f, 60.0f, SDL_Color{200, 200, 200, 0},
                                     shortSaveName, E_GAME_STATE::STARTSAVE)
            .function = [&]() {
            gameEngine_.gameState = E_GAME_STATE::RUNNING;
            usingMouse = false;
            Light::resetLight();
            scene_.reset();
            scene_ = std::make_unique<Scene>();
            setupScene(*scene_, gameEngine_, saves.c_str());
            mainCamera = &scene_->getGameObject("world/MainCamera");

            SDL_ShowCursor(false);
            SDL_SetRelativeMouseMode(SDL_TRUE);
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
        gameEngine_.gameState = E_GAME_STATE::STARTING;
    };

#pragma endregion
}

void Demo::loadATH(t_RessourcesManager &ressourceManager)
{
    float halfWidth = gameEngine_.getWinSize().width / 2.f;
    float halfHeight = gameEngine_.getWinSize().heigth / 2.f;
    float crosshairSize = gameEngine_.getWinSize().heigth / 15;
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
    enemiesContener = &scene_->add<GameObject>(scene_->getWorld(), GameObjectCreateArg{"EnemiesContener"});

    GameObjectCreateArg Ennemy1GameObjectArg{"Ennemy"};

    ModelCreateArg modelArg{&ressourceManager.get<Shader>("ColorWithLight"),
                          {&ressourceManager.get<Material>("GreenMaterial")},
                          &ressourceManager.get<Mesh>("Sphere"),
                          "ColorWithLight",
                          {"GreenMaterial"},
                          "Sphere"};

    GameObject& enemy1 = scene_->add<GameObject>(scene_->getWorld(), Ennemy1GameObjectArg);

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

    GameObject& particleGO = scene_->add<GameObject>(scene_->getWorld(), GameObjectCreateArg{"ParticleContener", {{0.f, 10.f, 0.f}}});
    particleGO.addComponent<ParticuleGenerator>(particalArg);
    particleGO.addComponent<LifeDuration>(10.f);
    
    scene_->add<GameObject>(scene_->getWorld(), GameObjectCreateArg{"DecalContenor", {{0.f, 0.f, 0.f}}}).addComponent<MaxElementConteneur>(10);
}

void Demo::updateControl()
{
    testLifePLayer -= 0.1;

    if (Input::keyboard.getKeyState(SDL_SCANCODE_ESCAPE) == 1)
    {
        if (gameEngine_.gameState == E_GAME_STATE::RUNNING)
        {
            gameEngine_.gameState = E_GAME_STATE::PAUSE;
            usingMouse = true;

            SDL_ShowCursor(true);
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
        else if (gameEngine_.gameState == E_GAME_STATE::PAUSE)
        {
            gameEngine_.gameState = E_GAME_STATE::RUNNING;
            usingMouse = false;

            SDL_ShowCursor(false);
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
        else if (gameEngine_.gameState == E_GAME_STATE::OPTION || gameEngine_.gameState == E_GAME_STATE::LOADSAVE || gameEngine_.gameState == E_GAME_STATE::STARTSAVE)
        {
            gameEngine_.gameState = E_GAME_STATE::STARTING;
        }
        else if (gameEngine_.gameState == E_GAME_STATE::STARTING)
        {
            gameEngine_.gameState = E_GAME_STATE::EXIT;
        }
    }

    if (Input::keyboard.isDown[SDL_SCANCODE_F3] && !flagF1IsDown)
    {
        usingMouse = !usingMouse;
        flagF1IsDown = true;

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
    else
    {
        flagF1IsDown = Input::keyboard.isDown[SDL_SCANCODE_F3];
    }

    if (Input::keyboard.getKeyState(SDL_SCANCODE_F6) == 1)
    {
        saveScene(*scene_, gameEngine_, "./ressources/saves/testtest.xml");
    }
}
