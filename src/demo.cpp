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

#include "GE/Physics/ColliderShape/SphereCollider.hpp"
#include "GE/Physics/ColliderShape/OrientedBoxCollider.hpp"
#include "GE/Physics/ColliderShape/CapsuleCollider.hpp"
#include "GE/Physics/ColliderShape/AABBCollider.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Ressources/Saves.hpp"

#include "Game/define.h"
#include "Game/BarIndicatorController.hpp"
#include "Game/CircularEntitiesSpawner.hpp"
#include "Game/PushedOnShoot.hpp"
#include "Game/Nexus.hpp"
#include "Game/ParticuleGenerator.hpp"
#include "Game/MaxElementConteneur.hpp"
#include "Game/PushedOnShoot.hpp"
#include "Game/GroundController.hpp"
#include "Game/WaveManager.hpp"
#include "Game/DayNightCycle.hpp"
#include "Game/Firearm.hpp"
#include "Game/Sniper.hpp"
#include "Game/SubMachineGun.hpp"
#include "Game/Shotgun.hpp"
#include "Game/LootMachine.hpp"
#include "Game/Loot.hpp"
#include "Game/LevitationMovement.hpp"

#include "../src/stb_image.h"

#ifndef DNEDITOR
#include "GE/LowRenderer/EditorTools/Editor.hpp"
using namespace Engine::LowRenderer::EditorTools;
#endif

#include "GE/Core/System/RenderingSystem.hpp"
#include "GE/Core/System/ScriptSystem.hpp"
#include "Game/PlayerController.hpp"
#include "Game/EnnemyController.hpp"
#include "Game/Checkpoint.hpp"
#include "GE/Ressources/SoundPlayer.hpp"
#include "GE/Ressources/Sound.hpp"

#include <SDL2/SDL_mouse.h>
#include <vector>
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

Demo::Demo(Engine::GE& gameEngine)
    :   _gameEngine         {gameEngine},
        _scene              {},
        flagleftClicIsDown  {false},
        usingMouse          {true},
        dirCamera           {0.f, 0.f, -1.f}
{
    _scene = std::make_unique<Scene>();
    _gameEngine.ressourceManager_.use();

    Scene::_currentScene = &_scene;

    loadRessources(_gameEngine.ressourceManager_);
    loadSounds(_gameEngine.ressourceManager_);
    loadCamera();
    loadEntity(_gameEngine.ressourceManager_);
    loadLights(_gameEngine.ressourceManager_);
    loadATH(_gameEngine.ressourceManager_);
    loadEnemies(_gameEngine.ressourceManager_);
    // setupScene(*_scene, _gameEngine, "./ressources/saves/testtest.xml");
    // mainCamera = &_scene->getGameObject("world/MainCamera");
    loadReferential(_gameEngine.ressourceManager_);
    loadUI(_gameEngine.ressourceManager_);

    ScriptSystem::start();

    // setupScene(_scene, _gameEngine, "./ressources/saves/setup.xml");

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    UISystem::isActive = true;
}

void Demo::update() noexcept
{
    SDL_ShowCursor(usingMouse);
    SDL_CaptureMouse(usingMouse ? SDL_FALSE : SDL_TRUE);
    SDL_SetRelativeMouseMode(usingMouse ? SDL_FALSE : SDL_TRUE);

    UISystem::update(_gameEngine);

    updateControl();

    if (_gameEngine.gameState == E_GAME_STATE::RUNNING)
    {
        ScriptSystem::update();
        _scene->update();
    }

#ifndef DNEDITOR
    Editor::update(*_scene, _gameEngine);
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
    ressourceManager.add<Shader>("Color", "./ressources/shader/vTextureOnlyWithProjection.vs", "./ressources/shader/fColorOnly.fs", AMBIANTE_COLOR_ONLY);
    ressourceManager.add<Shader>("TextureOnly", "./ressources/shader/vTextureOnlyWithProjection.vs", "./ressources/shader/fTextureOnly.fs");
    ressourceManager.add<Shader>("LightAndTexture", "./ressources/shader/vTexture2.vs", "./ressources/shader/fTexture2.fs", LIGHT_BLIN_PHONG);

    MaterialAndTextureCreateArg matDefault;
    matDefault.name = "DefaultMaterial";
    matDefault.pathDiffuseTexture = nullptr;
    matDefault.flipTexture = false;

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "PinkMaterial";
    matDefault.comp.ambient.rgbi = Vec4{1.f, 0.f, 1.f, 1.f};

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "RedMaterial";
    matDefault.comp.ambient.rgbi = Vec4{1.f, 0.f, 0.f, 1.f};
    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "GreenMaterial";
    matDefault.comp.ambient.rgbi = Vec4{0.f, 1.f, 0.f, 1.f};

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "BlueMaterial";
    matDefault.comp.ambient.rgbi = Vec4{0.f, 0.f, 1.f, 1.f};

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "BlackMaterial";
    matDefault.comp.ambient.rgbi = Vec4{0.f, 0.f, 0.f, 1.f};

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "BrownMaterial";
    matDefault.comp.ambient.rgbi = Vec4{0.45f, 0.25f, 0.04f, 1.f};

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    ressourceManager.add<Mesh>("Cube", Mesh::createCube(1));
    ressourceManager.add<Mesh>("Sphere", Mesh::createSphere(25, 25));
    ressourceManager.add<Mesh>("Cylinder", Mesh::createCylindre(25));
    ressourceManager.add<Mesh>("Plane", Mesh::createPlane());
    ressourceManager.add<Mesh>("PlaneZ", Mesh::createPlane(1.f, 0, 0, Mesh::Axis::Z));

    MaterialAndTextureCreateArg matBulletHole;
    matBulletHole.name                 = "BulletHole";
    matBulletHole.pathDiffuseTexture   = "./ressources/texture/bulletHole.png";

    {
        std::vector<Material> material;
        material.emplace_back(matBulletHole);
        ressourceManager.add<std::vector<Material>>(matBulletHole.name, std::move(material));
    }

    loadRockRessource          (ressourceManager);
    loadTreeRessource          (ressourceManager);
    loadSkyboxRessource        (ressourceManager);
    loadGunRessource           (ressourceManager);
    loadPseudoRessource        (ressourceManager);
    loadPlayerRessource        (ressourceManager);
    //loadSpotLightRessource     (ressourceManager);
    //loadTowerRessource         (ressourceManager);
    loadNexusRessource          (ressourceManager);  
    loadGroundRessource         (ressourceManager);
    loadCrateRessource          (ressourceManager);  
    loadGlassRessource          (ressourceManager);
    loadFogRessource            (ressourceManager);
}
void Demo::loadSounds(t_RessourcesManager &rm)
{
    SoundPlayer::initialize();
    //SoundPlayer::listDevices();
    rm.add<Sound>("pistol","./ressources/sound/pistol.wav").setGain(_effectsVolume);
    rm.add<Sound>("chaingun","./ressources/sound/chaingun.wav").setGain(_effectsVolume);
    rm.add<Sound>("Sniper","./ressources/sound/rifle.wav").setGain(_effectsVolume);
    rm.add<Sound>("Shotgun","./ressources/sound/shotgun.wav").setGain(_effectsVolume);
    rm.add<Sound>("Machinegun","./ressources/sound/machinegun.wav").setGain(_effectsVolume);
    
    Sound& bg = rm.add<Sound>("background","./ressources/music/dark_forces.wav");
    bg.setGain(_musicVolume);
    bg.setLooping(true);

    //play background sound at startup
    SoundPlayer::play(bg);
}

void Demo::loadRockRessource(t_RessourcesManager &ressourceManager)
{
    Attrib attrib;
    std::vector<Shape> shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./ressources/obj/stone.obj", &attrib, &shape, &materialAttribs);

    ressourceManager.add<Mesh>("RockMesh", attrib, shape);

    {
        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign(materialAttribs.begin(), materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("RockMaterials", std::move(material));
    }
}

void Demo::loadTreeRessource(t_RessourcesManager &ressourceManager)
{
    Attrib attrib;
    std::vector<Shape> shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./ressources/obj/Tree.obj", &attrib, &shape, &materialAttribs);

    ressourceManager.add<Mesh>("TreeMesh", attrib, shape);

    {
        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign(materialAttribs.begin(), materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("TreeMaterials", std::move(material));
    }
}

void Demo::loadSkyboxRessource(t_RessourcesManager &ressourceManager)
{
    MaterialAndTextureCreateArg matSkybox;
    matSkybox.name = "SkyBox";
    matSkybox.pathDiffuseTexture = "./ressources/texture/arrakisday/skb.bmp";
    matSkybox.flipTexture = false;
    matSkybox.filterType = E_FilterType::LINEAR;

    ressourceManager.add<Shader>("SkyboxShader", "./ressources/shader/vSkybox.vs", "./ressources/shader/fLightingSkybox.fs", SKYBOX | LIGHT_BLIN_PHONG);
    ressourceManager.add<std::vector<Material>>("SkyboxMaterial", std::vector<Material>{matSkybox});
    ressourceManager.add<Mesh>("SkyboxMesh", "./ressources/obj/skybox.obj");
}

void Demo::loadGunRessource(t_RessourcesManager &ressourceManager)
{
    /*Sniper*/
    {
        Attrib                      attrib;
        std::vector<Shape>          shape;
        std::vector<MaterialAttrib> materialAttribs;

        loadObjWithMTL("./ressources/obj/World_war_II_Sniper_gun_3d_models.obj", &attrib, &shape, &materialAttribs);

        ressourceManager.add<Mesh>("SniperMesh", attrib, shape);

        {
            std::vector<Material> material;
            material.reserve(materialAttribs.size());
            /*Instanciate material vector with data read on materalAtribs*/
            material.assign (materialAttribs.begin(),materialAttribs.end());
            ressourceManager.add<std::vector<Material>>("SniperMaterials", std::move(material));
        }
    }

    /*Submachine gun*/
    {
        Attrib                      attrib;
        std::vector<Shape>          shape;
        std::vector<MaterialAttrib> materialAttribs;

        loadObjWithMTL("./ressources/obj/PPSH-41.obj", &attrib, &shape, &materialAttribs);

        ressourceManager.add<Mesh>("SubMachineGunMesh", attrib, shape);

        {
            std::vector<Material> material;
            material.reserve(materialAttribs.size());
            /*Instanciate material vector with data read on materalAtribs*/
            material.assign (materialAttribs.begin(),materialAttribs.end());
            ressourceManager.add<std::vector<Material>>("SubMachineGunMaterials", std::move(material));
        }
    }

    /*Shotgun*/
    {
        Attrib                      attrib;
        std::vector<Shape>          shape;
        std::vector<MaterialAttrib> materialAttribs;

        loadObjWithMTL("./ressources/obj/m870.obj", &attrib, &shape, &materialAttribs);

        ressourceManager.add<Mesh>("ShotgunMesh", attrib, shape);

        {
            std::vector<Material> material;
            material.reserve(materialAttribs.size());
            /*Instanciate material vector with data read on materalAtribs*/
            material.assign (materialAttribs.begin(),materialAttribs.end());
            ressourceManager.add<std::vector<Material>>("ShotgunMaterials", std::move(material));
        }
    }
}

void Demo::loadPseudoRessource(t_RessourcesManager &ressourceManager)
{
    FontCreateArg fontArg;
    fontArg.fontRoot = "./ressources/font/COURIER.ttf";
    fontArg.fontSize = 25;

    Font &font = ressourceManager.add<Font>("COURIER", fontArg);

    TextCreateArg textArg;
    textArg.text = "Mjr. Cavalsky";
    textArg.pFont = &font;
    textArg.color = {0.5f, 0.5f, 0.f, 1.f};

    MaterialCreateArg matText;
    matText.name = "PseudoMaterial";
    matText.pTexture = std::make_unique<Text>(textArg);
    matText.comp.ambient.ki = 1.f;
    matText.comp.diffuse.ki = 0.f;
    matText.comp.specular.ki = 0.f;

    {
        std::vector<Material> material;
        material.emplace_back(matText);
        ressourceManager.add<std::vector<Material>>(matText.name, std::move(material));
    }
}

void Demo::loadPlayerRessource(t_RessourcesManager &ressourceManager)
{
    Attrib attrib;
    std::vector<Shape> shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./ressources/obj/CODSoldier.obj", &attrib, &shape, &materialAttribs);

    ressourceManager.add<Mesh>("Soldier1Mesh", attrib, shape);

    {
        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign(materialAttribs.begin(), materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("Soldier1Materials", std::move(material));
    }
}

void Demo::loadTowerRessource(t_RessourcesManager &ressourceManager)
{
    Attrib attrib;
    std::vector<Shape> shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./ressources/obj/guardTower.obj", &attrib, &shape, &materialAttribs);

    ressourceManager.add<Mesh>("GuardTowerMesh", attrib, shape);

    {
        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign(materialAttribs.begin(), materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("GuardTowerMaterials", std::move(material));
    }
}

void Demo::loadSpotLightRessource(t_RessourcesManager &ressourceManager)
{
    Attrib attrib;
    std::vector<Shape> shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./ressources/obj/spotLight.obj", &attrib, &shape, &materialAttribs);

    ressourceManager.add<Mesh>("SpotLightMesh", attrib, shape);

    {
        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign(materialAttribs.begin(), materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("SpotLightMaterial", std::move(material));
    }
}

void Demo::loadGroundRessource(t_RessourcesManager &ressourceManager)
{
    ressourceManager.add<Mesh>("GroundMesh" ,Mesh::createPlane(100));

    MaterialAndTextureCreateArg matGround;
    matGround.name = "Ground";
    matGround.comp.shininess = 1.f;
    matGround.comp.specular.rgbi = {1.f, 1.f, 1.f, 0.f};
    matGround.pathDiffuseTexture = "./ressources/texture/arrakisday/arrakisday_dn.tga";
    matGround.wrapType = E_WrapType::MIRRORED_REPEAT;

    {
        std::vector<Material> material;
        material.emplace_back(matGround);
        ressourceManager.add<std::vector<Material>>(matGround.name, std::move(material));
    }
}

void Demo::loadCrateRessource(t_RessourcesManager &ressourceManager)
{
    MaterialAndTextureCreateArg matCrate;
    matCrate.name = "CrateMaterial";
    matCrate.comp.specular.rgbi = {1.f, 1.f, 1.f, 0.1f};
    matCrate.pathDiffuseTexture = "./ressources/texture/crate.png";

    {
        std::vector<Material> material;
        material.emplace_back(matCrate);
        ressourceManager.add<std::vector<Material>>(matCrate.name, std::move(material));
    }
}

void Demo::loadNexusRessource(t_RessourcesManager& ressourceManager)
{
    Attrib                      attrib;
    std::vector<Shape>          shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./ressources/obj/Crystal_low.obj", &attrib, &shape, &materialAttribs);

    ressourceManager.add<Mesh>("NexusMesh" , attrib, shape);

    MaterialAndTextureCreateArg matNexus;
    matNexus.name = "NexusMaterial";
    matNexus.comp.shininess = 32.f;
    matNexus.comp.specular.rgbi = {1.f, 1.f, 1.f, 1.0f};
    matNexus.pathDiffuseTexture = "./ressources/texture/text2.png";

    {
        std::vector<Material> material;
        material.emplace_back(matNexus);
        ressourceManager.add<std::vector<Material>>(matNexus.name, std::move(material));
    }
}

void Demo::loadGlassRessource          (t_RessourcesManager& ressourceManager)
{
    MaterialAndTextureCreateArg matGlass;
    matGlass.name = "GlassMaterial";
    matGlass.comp.specular.rgbi = {1.f, 1.f, 1.f, 0.1f};
    matGlass.pathDiffuseTexture = "./ressources/texture/glass.png";

    {
        std::vector<Material> material;
        material.emplace_back(matGlass);
        ressourceManager.add<std::vector<Material>>(matGlass.name, std::move(material));
    }
}

void Demo::loadFogRessource           (Engine::Ressources::t_RessourcesManager& ressourceManager)
{
    MaterialAndTextureCreateArg matFog;
    matFog.name = "FogMaterials";
    matFog.comp.specular.rgbi = {1.f, 1.f, 1.f, 0.1f};
    matFog.comp.ambient.ki     = 1.f;
    matFog.comp.diffuse.ki     = 0.f;
    matFog.comp.specular.ki    = 0.f;
    matFog.pathDiffuseTexture = "./ressources/texture/billboardCloud.jpg";

    {
        std::vector<Material> material;
        material.emplace_back(matFog);
        ressourceManager.add<std::vector<Material>>(matFog.name, std::move(material));
    }

    ressourceManager.add<Mesh>("BillBoardFogMesh", Mesh::createPlane(0.25, 3, 3, Mesh::Axis::Z));
    ressourceManager.add<Shader>("BillBoardFogShader", "./ressources/shader/vCloud.vs", "./ressources/shader/fCloud.fs", SCALE_TIME_ACC);
}

void Demo::loadRock                   (t_RessourcesManager& ressourceManager, unsigned int number)
{
    GameObjectCreateArg rockGameObject{"Rocks",
                                       {{0.f, 0.f, 0.f},
                                        {0.f, 0.f, 0.f},
                                        {1.f, 1.f, 1.f}}};

    std::vector<Material> &vecMaterials = ressourceManager.get<std::vector<Material>>("RockMaterials");

    ModelCreateArg rockModelArg{&ressourceManager.get<Shader>("LightAndTexture"),
                                &vecMaterials,
                                &ressourceManager.get<Mesh>("RockMesh"),
                                "LightAndTexture",
                                "RockMaterials",
                                "RockMesh"};

    GameObject &rockContener = _scene->add<GameObject>(_scene->getWorld(), rockGameObject);

    /*Create rock with random size, position and rotation and add it on rock contener*/
    for (size_t i = 0; i < number; i++)
    {
        rockGameObject.name = "Rock" + std::to_string(i);
        rockGameObject.transformArg.position.x = Random::ranged<float>(-250.f, 250.f);
        rockGameObject.transformArg.position.z = Random::ranged<float>(-250.f, 250.f);
        rockGameObject.transformArg.rotation.y = Random::ranged<float>(360.f * M_PI / 180.f);
        rockGameObject.transformArg.rotation.x = Random::ranged<float>(360.f * M_PI / 180.f);

        rockGameObject.transformArg.scale = {Random::ranged<float>(2.f, 8.f), Random::ranged<float>(2.f, 8.f), Random::ranged<float>(2.f, 8.f)};

        _scene->add<GameObject>(rockContener, rockGameObject).addComponent<Model>(rockModelArg);
    }
}

void Demo::loadTree(t_RessourcesManager &ressourceManager, unsigned int number)
{
    GameObjectCreateArg treeGameObject{"Trees",
                                       {{0.f, 0.f, 0.f},
                                        {0.f, 0.f, 0.f},
                                        {1.f, 1.f, 1.f}}};

    GameObject& treeContener = _scene->add<GameObject>(_scene->getWorld(), treeGameObject);
    std::vector<Material> &vecMaterials = ressourceManager.get<std::vector<Material>>("TreeMaterials");
    ModelCreateArg treeModelArg{&ressourceManager.get<Shader>("LightAndTexture"),
                                &vecMaterials,
                                &ressourceManager.get<Mesh>("TreeMesh"),
                                "LightAndTexture",
                                "TreeMaterials",
                                "TreeMesh"};

    /*Create tree with random size, position and rotation and add it on tre contener*/
    for (size_t i = 0; i < number; i++)
    {
        treeGameObject.name = "Tree" + std::to_string(i);
        treeGameObject.transformArg.position.x = Random::ranged<float>(-250.f, 250.f);
        treeGameObject.transformArg.position.z = Random::ranged<float>(-250.f, 250.f);
        treeGameObject.transformArg.rotation.y = Random::ranged<float>(360.f * M_PI / 180.f);
        float globalScale = Random::ranged<float>(8.f, 12.f);
        treeGameObject.transformArg.scale += globalScale;

        treeGameObject.transformArg.scale = {Random::ranged<float>(2.f, 8.f), Random::ranged<float>(2.f, 8.f), Random::ranged<float>(2.f, 8.f)};

        _scene->add<GameObject>(treeContener, treeGameObject).addComponent<Model>(treeModelArg);
    }
}

void Demo::loadSkybox(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg skyboxArgGameObject{"Skybox",
                                            {{0.f, 0.f, 0.f},
                                             {0.f, 0.f, 0.f},
                                             {10.f, 10.f, 10.f}}};

    ModelCreateArg skyboxArg{&ressourceManager.get<Shader>("SkyboxShader"),
                             &ressourceManager.get<std::vector<Material>>("SkyboxMaterial"),
                             &ressourceManager.get<Mesh>("SkyboxMesh"),
                             "SkyboxShader",
                             "SkyboxMaterial",
                             "SkyboxMesh", true, false};

    _scene->add<GameObject>(_scene->getWorld(), skyboxArgGameObject).addComponent<Model>(skyboxArg);
}

void Demo::loadPlayer(t_RessourcesManager &ressourceManager)
{
    /*Load the player*/
    GameObjectCreateArg playerGameObject    {"Players",
                                            {{0.f, 0.f, 0.f},
                                            {0.f, 0.f, 0.f},
                                            {1.f, 1.f, 1.f}}};

    GameObject& playerContener = _scene->add<GameObject>(_scene->getWorld(), playerGameObject);

    std::vector<Material> &vecMaterials = ressourceManager.get<std::vector<Material>>("Soldier1Materials");

    playerGameObject.name = "Player1";
    playerGameObject.transformArg.position = {2.f, 10.f, 2.f};
    GameObject& player1GO = _scene->add<GameObject>(playerContener, playerGameObject);
    PlayerController& playerControllerPlayer1 = player1GO.addComponent<PlayerController>();
    player1GO.addComponent<PhysicalObject>().setMass(1);
    player1GO.addComponent<SphereCollider>().getLocalSphere().setRadius(5.f);
    player1GO.getComponent<SphereCollider>()->setBounciness(0.f);
    player1GO.getComponent<SphereCollider>()->setFriction(0.70f);

    playerGameObject.name = "Skin";
    playerGameObject.transformArg.position = {0.f, -10.f, -2.f};
    playerGameObject.transformArg.rotation = {0.f, 0.f, 0.f};
    playerGameObject.transformArg.scale = {0.18f, 0.18f, 0.18f};

    ModelCreateArg soldierModelArg{&ressourceManager.get<Shader>("LightAndTexture"),
                                   &vecMaterials,
                                   &ressourceManager.get<Mesh>("Soldier1Mesh"),
                                   "LightAndTexture",
                                   "Soldier1Materials",
                                   "Soldier1Mesh"};

    GameObject& skinPlayer1GO = _scene->add<GameObject>(player1GO, playerGameObject);
    skinPlayer1GO.addComponent<Model>(soldierModelArg);
    
    //load flashlight
    GameObjectCreateArg flashlightGameObject    {"FlashLight",
                                                {{0.f, 0.f, 1.7f}, 
                                                {0.f, 0.f, -M_PI / 10.f}}};

    SpotLightCreateArg lightArg{{1.f, 1.f, 1.f, 0.f},
                                 {1.f, 1.f, 1.f, 0.7f},
                                 {1.f, 1.f, 1.f, 0.3f},
                                 0.3f, 0.001f, 0.f,
                                 20.7f, 27.5f, 
                                 false};

    _scene->add<GameObject>(player1GO, flashlightGameObject).addComponent<SpotLight>(lightArg);

    //load guns

    //Sniper
    {
        GameObjectCreateArg sniperGameObject    {"Sniper",
                                                {{-0.5f, -1.5f, 3.2f},
                                                {0.f, -M_PI_2, 0.f},
                                                {0.02f, 0.02f, 0.02f}}};

        std::vector<Material> &vecMaterialsGun = ressourceManager.get<std::vector<Material>>("SniperMaterials");

        ModelCreateArg sniperModelArg{  &ressourceManager.get<Shader>("LightAndTexture"),
                                        &vecMaterialsGun,
                                        &ressourceManager.get<Mesh>("SniperMesh"),
                                        "LightAndTexture",
                                        "SniperMaterials",
                                        "SniperMesh"};
            
        GameObject& sniperGO = _scene->add<GameObject>(player1GO, sniperGameObject);
        sniperGO.addComponent<Model>(sniperModelArg);
        Firearm& sniperComponent = sniperGO.addComponent<Sniper>(10.f, 1000.f, 1, 1.f, 50, 0.2f,&ressourceManager.get<Sound>("Sniper"));
        playerControllerPlayer1.addFirearm(&sniperComponent);
    }
    //Shotgun
    {
        GameObjectCreateArg shotgunGameObject{"shotgun",{{-0.5f, -1.5f, 3.8f}, {0.f, M_PI, 0.f}}};
        std::vector<Material>& vecMaterialsShotgun = ressourceManager.get<std::vector<Material>>("ShotgunMaterials");

        ModelCreateArg shotgunModelArg{&ressourceManager.get<Shader>("LightAndTexture"),
                                        &vecMaterialsShotgun,
                                        &ressourceManager.get<Mesh>("ShotgunMesh"),
                                        "LightAndTexture",
                                        "ShotgunMaterials",
                                        "ShotgunMesh"};

        GameObject& shotgunGO = _scene->add<GameObject>(player1GO, shotgunGameObject);
        shotgunGO.addComponent<Model>(shotgunModelArg);
        Firearm& shotgunComponent = shotgunGO.addComponent<Shotgun>(10.f, 1000.f, 10, 1.f, 50, 0.2f, 0.5,&ressourceManager.get<Sound>("Shotgun"));
        playerControllerPlayer1.addFirearm(&shotgunComponent);
    }
    //SubMachinegun
    {
        GameObjectCreateArg subMachineGunGameObject {"SubMachineGun",
                                                    {{-0.5f, -1.5f, 3.2f},
                                                    {0.f, M_PI, 0.f},
                                                    {0.3f, 0.3f, 0.3f}}};

        std::vector<Material>& vecMaterialsSubMachineGun = ressourceManager.get<std::vector<Material>>("SubMachineGunMaterials");

        ModelCreateArg subMachineGunModelArg{&ressourceManager.get<Shader>("LightAndTexture"),
                                        &vecMaterialsSubMachineGun,
                                        &ressourceManager.get<Mesh>("SubMachineGunMesh"),
                                        "LightAndTexture",
                                        "SubMachineGunMaterials",
                                        "SubMachineGunMesh"};

        GameObject& subMachineGunGO = _scene->add<GameObject>(player1GO, subMachineGunGameObject);
        subMachineGunGO.addComponent<Model>(subMachineGunModelArg);
        Firearm& subMachineGunComponent = subMachineGunGO.addComponent<SubMachineGun>(10.f, 1000.f, 1, 1.f, 50, 0.1f,&ressourceManager.get<Sound>("Machinegun"));
        playerControllerPlayer1.addFirearm(&subMachineGunComponent);
    }

    //load billboards
    std::vector<Material> &vecMaterialsPseudo = ressourceManager.get<std::vector<Material>>("PseudoMaterial");
    Size textureSize = vecMaterialsPseudo[0].getPDiffuseTexture()->getSize();

    GameObjectCreateArg pseudoGameObject{"Pseudo",
                                         {{0.0f, 5.0f, 0.0f},
                                          {0.f, 0.f, 0.f},
                                          {-textureSize.width / 10.f, 1.f, textureSize.heigth / 10.f}}};

    ModelCreateArg planeArg{&ressourceManager.get<Shader>("TextureOnly"),
                            &vecMaterialsPseudo,
                            &ressourceManager.get<Mesh>("Plane"),
                            "TextureOnly",
                            "PseudoMaterial",
                            "Plane", true, false};

    _scene->add<GameObject>(player1GO, pseudoGameObject).addComponent<BillBoard>(planeArg);
}

void Demo::loadTower(t_RessourcesManager &ressourceManager)
{
    //create the tower
    GameObjectCreateArg towerArgGameObject{"Tower",
                                           {{0.f, 0.f, 0.f},
                                            {0.f, 0.f, 0.f},
                                            {1.f, 1.f, 1.f}}};

    ModelCreateArg towerModelArg{&ressourceManager.get<Shader>("LightAndTexture"),
                                 &ressourceManager.get<std::vector<Material>>("GuardTowerMaterials"),
                                 &ressourceManager.get<Mesh>("GuardTowerMesh"),
                                 "LightAndTexture",
                                 "GuardTowerMaterials",
                                 "GuardTowerMesh"};

    GameObject& towerGO = _scene->add<GameObject>(_scene->getWorld(), towerArgGameObject);
    towerGO.addComponent<Model>(towerModelArg);

    //create projector
    GameObjectCreateArg spotLightArgGameObject{"SpotLight",
                                               {{0.0f, 48.0f, -5.0f},
                                                {M_PI, 0.f, 0.f},
                                                {0.05f, 0.05f, 0.05f}}};

    ModelCreateArg spotLightModelArg{&ressourceManager.get<Shader>("LightAndTexture"),
                                     &ressourceManager.get<std::vector<Material>>("SpotLightMaterial"),
                                     &ressourceManager.get<Mesh>("SpotLightMesh"),
                                     "LightAndTexture",
                                     "SpotLightMaterial",
                                     "SpotLightMesh"};

    GameObject& spotLightGO = _scene->add<GameObject>(towerGO, spotLightArgGameObject);
    spotLightGO.addComponent<Model>(spotLightModelArg);

    //create light
    GameObjectCreateArg lightArgGameObject{"light",
                                           {{0.0f, 90.0f, -50.0f},
                                            {0, 0.f, 0.f},
                                            {21.f, 21.f, 21.f}}};

    ModelCreateArg spherModelarg{&ressourceManager.get<Shader>("ColorWithLight"),
                                &ressourceManager.get<std::vector<Material>>("DefaultMaterial"),
                                &ressourceManager.get<Mesh>("Sphere"),
                                "ColorWithLight",
                                "PinkMaterial",
                                "Sphere"};

    SpotLightCreateArg lightSpotArg {{1.f, 1.f, 1.f, 0.f}, 
                                     {1.f, 1.f, 1.f, 1.f}, 
                                     {1.f, 1.f, 1.f, 1.f},
                                     0.f, 0.005f, 0.f,
                                     /*{0.f, -0.5f, 0.6f}, DIRECTION*/
                                     5.f, 7.f};

    GameObject& lightGO = _scene->add<GameObject>(spotLightGO, lightArgGameObject);
    lightGO.addComponent<Model>(spherModelarg);
    lightGO.addComponent<SpotLight>(lightSpotArg).enable(true);
}

void Demo::loadGround(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg groundArgGameObject{"Ground",
                                            {{0.f, -20.f, 0.f},
                                             {0.f, 0.f, 0.f},
                                             {1000.f, 1.f, 1000.f}}};

    ModelCreateArg groundArg{&ressourceManager.get<Shader>("LightAndTexture"),
                             &ressourceManager.get<std::vector<Material>>("Ground"),
                             &ressourceManager.get<Mesh>("GroundMesh"),
                             "LightAndTexture",
                             "Ground",
                             "GroundMesh",
                             true,
                             false};

    GameObject& ground = _scene->add<GameObject>(_scene->getWorld(), groundArgGameObject);
    ground.addComponent<Model>(groundArg);
    ground.addComponent<GroundController>();
    ground.addComponent<OrientedBoxCollider>();
    ground.setTag("Ground");
}

void Demo::loadFog           (Engine::Ressources::t_RessourcesManager& ressourceManager, unsigned int number)
{
   GameObjectCreateArg fogGameObjectArg  {"FogContener"};

    GameObject& fogContener = _scene->add<GameObject>(_scene->getWorld(), fogGameObjectArg);

    std::vector<Material>& vecMaterials = ressourceManager.get<std::vector<Material>>("FogMaterials");
    
    ModelCreateArg fogModelArg{&ressourceManager.get<Shader>("BillBoardFogShader"),
                            &vecMaterials,
                            &ressourceManager.get<Mesh>("BillBoardFogMesh"),
                            "BillBoardFogShader",
                            "FogMaterials",
                            "BillBoardFogMesh",
                            true, false, false};
    
    /*Create tree with random size, position and rotation and add it on tre contener*/
    for (size_t i = 0; i < number; i++)
    {
        fogGameObjectArg.name = "Fog" + std::to_string(i);
        fogGameObjectArg.transformArg.position.x = Random::ranged<float>(-250.f, 250.f);
        fogGameObjectArg.transformArg.position.z = Random::ranged<float>(-250.f, 250.f);
        fogGameObjectArg.transformArg.rotation.y = Random::ranged<float>(360.f * M_PI / 180.f);
        fogGameObjectArg.transformArg.scale = {Random::ranged<float>(50.f, 100.f), Random::ranged<float>(20.f, 55.f), 1.f};
        fogGameObjectArg.transformArg.position.y = fogGameObjectArg.transformArg.scale.y / 2.f;

        _scene->add<GameObject>(fogContener, fogGameObjectArg).addComponent<BillBoard>(fogModelArg);
    }
}

void Demo::loadLootMachin              (t_RessourcesManager& ressourceManager)
{
    GameObjectCreateArg lootMachinArgGameObject{"LootMachine",
                                            {{20.f, 5.f, 20.f},
                                             {0.f, M_PI_4, 0.f},
                                             {1.f, 1.f, 1.f}}};

    GameObject& lootMachin = _scene->add<GameObject>(_scene->getWorld(), lootMachinArgGameObject);
    lootMachin.addComponent<LootMachine>();

    float wrapHeight = 10.f;
    float wrapWidth = 4.f;
    float wrapDepth = 2.5f;
    float wrapThickness = 0.3f;

    /*Lever*/
    GameObjectCreateArg leverArgGameObject{"Lever",
                                            {{wrapWidth - wrapWidth / 4.f, 0.f, 0.f},
                                            {0, 0.f, 0.f},
                                            {1.f, 1.f, 1.f}}};

    GameObject& lever = _scene->add<GameObject>(lootMachin, leverArgGameObject);

    GameObjectCreateArg cylinderArgGameObject{"Cylinder",
                                            {{0.f, cylinderArgGameObject.transformArg.scale.z / 2.f, 0.f},
                                            {M_PI_2, 0.f, 0.f},
                                            {.3f, 0.3f, 3.f}}};
                                             
    ModelCreateArg cylinderArg{&ressourceManager.get<Shader>("LightAndTexture"),
                             &ressourceManager.get<std::vector<Material>>("BlackMaterial"),
                             &ressourceManager.get<Mesh>("Cylinder"),
                             "LightAndTexture",
                             "BlackMaterial",
                             "Cylinder", 
                            true, false};

    _scene->add<GameObject>(lever, cylinderArgGameObject).addComponent<Model>(cylinderArg);

     GameObjectCreateArg sphereArgGameObject{"Sphere",
                                            {{0.f, cylinderArgGameObject.transformArg.scale.z, 0.f},
                                            {0, 0.f, 0.f},
                                            {1.f, 1.f, 1.f}}};
                                             
    ModelCreateArg sphereArg{&ressourceManager.get<Shader>("LightAndTexture"),
                             &ressourceManager.get<std::vector<Material>>("RedMaterial"),
                             &ressourceManager.get<Mesh>("Sphere"),
                             "LightAndTexture",
                             "RedMaterial",
                             "Sphere"};

    _scene->add<GameObject>(lever, sphereArgGameObject).addComponent<Model>(sphereArg);

    GameObjectCreateArg cylinderBaseArgGameObject{"CylinderBase",
                                            {{-0.25f, 0.f, 0.f},
                                            {0.f, M_PI_2, 0.f},
                                            {0.7f, 0.7f, 1.5f}}};
                                             
    ModelCreateArg cylinderBaseArg{&ressourceManager.get<Shader>("LightAndTexture"),
                             &ressourceManager.get<std::vector<Material>>("BlackMaterial"),
                             &ressourceManager.get<Mesh>("Cylinder"),
                             "LightAndTexture",
                             "BlackMaterial",
                             "Cylinder", 
                            true, false};

    _scene->add<GameObject>(lever, cylinderBaseArgGameObject).addComponent<Model>(cylinderBaseArg);

    /*Mechanism*/
    GameObjectCreateArg mechanismArgGameObject{"Mechanism",
                                            {{0.f, 0.f, 0.f},
                                            {0, 0.f, 0.f},
                                            {1.f, 1.f, 1.f}}};

    GameObject& mechanism = _scene->add<GameObject>(lootMachin, mechanismArgGameObject);
    
    GameObjectCreateArg firstInclinedPlatformArgGameObject{"FirstInclinedPlatform",
                                            {{wrapWidth / 4.f, (wrapHeight / 8.f * 6) - wrapHeight / 2.f, 0.f},
                                            {0.f, 0.f, M_PI_4},
                                            {wrapWidth / 2.f, wrapThickness, wrapDepth}}};

    GameObjectCreateArg secondInclinedPlatformArgGameObject{"SecondInclinedPlatform",
                                            {{-wrapWidth / 4.f, (wrapHeight / 8.f * 5) - wrapHeight / 2.f, 0.f},
                                            {0.f, 0.f, -M_PI_4},
                                            {wrapWidth / 2.f, wrapThickness, wrapDepth}}};

    GameObjectCreateArg thirdInclinedPlatformArgGameObject{"ThirdInclinedPlatform",
                                            {{wrapWidth / 4.f, (wrapHeight / 8.f * 4) - wrapHeight / 2.f, 0.f},
                                            {0.f, 0.f, M_PI_4},
                                            {wrapWidth / 2.f, wrapThickness, wrapDepth}}};
                        
    GameObjectCreateArg forthInclinedPlatformArgGameObject{"ForthInclinedPlatform",
                                            {{-wrapWidth / 4.f, (wrapHeight / 8.f * 3) - wrapHeight / 2.f, 0.f},
                                            {0.f, 0.f, -M_PI_4},
                                            {wrapWidth / 2.f, wrapThickness, wrapDepth}}};
                            
    GameObjectCreateArg distributorInclinedPlatformArgGameObject{"DistributorInclinedPlatform",
                                            {{0.f, (wrapHeight / 8.f * 1) - wrapHeight / 2.f, 0.f},
                                            {M_PI_4, 0.f, 0.f},
                                            {wrapWidth, wrapThickness, wrapDepth}}};
                                             
    ModelCreateArg greenPlatformArg{&ressourceManager.get<Shader>("LightAndTexture"),
                             &ressourceManager.get<std::vector<Material>>("GreenMaterial"),
                             &ressourceManager.get<Mesh>("Cube"),
                            "LightAndTexture",
                            "GreenMaterial",
                            "Cube"};

    ModelCreateArg pinkPlatformArg{&ressourceManager.get<Shader>("LightAndTexture"),
                             &ressourceManager.get<std::vector<Material>>("PinkMaterial"),
                             &ressourceManager.get<Mesh>("Cube"),
                            "LightAndTexture",
                            "PinkMaterial",
                            "Cube"};

    ModelCreateArg redPlatformArg{&ressourceManager.get<Shader>("LightAndTexture"),
                             &ressourceManager.get<std::vector<Material>>("RedMaterial"),
                             &ressourceManager.get<Mesh>("Cube"),
                            "LightAndTexture",
                            "RedMaterial",
                            "Cube"};

    GameObject& firstInclinedPlatformGO = _scene->add<GameObject>(mechanism, firstInclinedPlatformArgGameObject);
    GameObject& secondInclinedPlatformGO = _scene->add<GameObject>(mechanism, secondInclinedPlatformArgGameObject);
    GameObject& thirdInclinedPlatformGO = _scene->add<GameObject>(mechanism, thirdInclinedPlatformArgGameObject);
    GameObject& fourthInclinedPlatformGO = _scene->add<GameObject>(mechanism, forthInclinedPlatformArgGameObject);
    GameObject& distributorInclinedPlatformGO = _scene->add<GameObject>(mechanism, distributorInclinedPlatformArgGameObject);

    firstInclinedPlatformGO.addComponent<Model>(greenPlatformArg);
    firstInclinedPlatformGO.addComponent<OrientedBoxCollider>().setBounciness(0.4f);

    secondInclinedPlatformGO.addComponent<Model>(pinkPlatformArg);
    secondInclinedPlatformGO.addComponent<OrientedBoxCollider>().setBounciness(0.4f);

    thirdInclinedPlatformGO.addComponent<Model>(greenPlatformArg);
    thirdInclinedPlatformGO.addComponent<OrientedBoxCollider>().setBounciness(0.4f);

    fourthInclinedPlatformGO.addComponent<Model>(pinkPlatformArg);
    fourthInclinedPlatformGO.addComponent<OrientedBoxCollider>().setBounciness(0.4f);

    distributorInclinedPlatformGO.addComponent<Model>(redPlatformArg);
    distributorInclinedPlatformGO.addComponent<OrientedBoxCollider>().setBounciness(1.f);


    /*Wrap*/
    GameObjectCreateArg wrapArgGameObject{"Wrap",
                                            {{0.f, 0.f, 0.f},
                                            {0, 0.f, 0.f},
                                            {1.f, 1.f, 1.f}}};

    GameObject& wrap = _scene->add<GameObject>(lootMachin, wrapArgGameObject);
    
                                             
    GameObjectCreateArg leftFaceArgGameObject{"leftFace",
                                            {{-wrapWidth / 2.f, 0.f, 0.f},
                                            {0.f, 0.f, 0.f},
                                            {wrapThickness, wrapHeight, wrapDepth}}};

    GameObjectCreateArg rightFaceArgGameObject{"rightFace",
                                            {{wrapWidth / 2.f, 0.f, 0.f},
                                            {0.f, 0.f, 0.f},
                                            {wrapThickness, wrapHeight, wrapDepth}}};

    GameObjectCreateArg backwardFaceArgGameObject{"BackwardFace",
                                            {{0.f, 0.f, -wrapDepth / 2.f},
                                            {0.f, 0.f, 0.f},
                                            {wrapWidth, wrapHeight, wrapThickness}}};

    GameObjectCreateArg topFaceArgGameObject{"TopFace",
                                            {{0.f, wrapHeight / 2.f, 0.f},
                                            {0.f, 0.f, 0.f},
                                            {wrapWidth, wrapThickness, wrapDepth}}};

    GameObjectCreateArg forwardTopFaceArgGameObject{"forwardTopFace",
                                            {{0.f, (wrapHeight) - wrapHeight / 2.f, wrapDepth / 2.f},
                                            {0.f, 0.f, 0.f},
                                            {wrapWidth, wrapHeight / 6.f, wrapThickness}}};

    GameObjectCreateArg forwardGlassFaceArgGameObject{"forwardGlassFace",
                                            {{0.f, wrapHeight / 10.f, wrapDepth / 2.f},
                                            {0.f, 0.f, 0.f},
                                            {wrapWidth, wrapHeight / 3.f * 2.f, wrapThickness}}};
                                            
    ModelCreateArg blackPlatformArg{&ressourceManager.get<Shader>("LightAndTexture"),
                             &ressourceManager.get<std::vector<Material>>("BlackMaterial"),
                             &ressourceManager.get<Mesh>("Cube"),
                            "LightAndTexture",
                            "BlackMaterial",
                            "Cube"};

    ModelCreateArg glassPlatformArg{&ressourceManager.get<Shader>("LightAndTexture"),
                             &ressourceManager.get<std::vector<Material>>("GlassMaterial"),
                             &ressourceManager.get<Mesh>("Cube"),
                            "LightAndTexture",
                            "GlassMaterial",
                            "Cube"};
    glassPlatformArg.isOpaque = false;

    _scene->add<GameObject>(wrap, rightFaceArgGameObject).addComponent<Model>(blackPlatformArg);
    _scene->add<GameObject>(wrap, leftFaceArgGameObject).addComponent<Model>(blackPlatformArg);
    _scene->add<GameObject>(wrap, backwardFaceArgGameObject).addComponent<Model>(blackPlatformArg);
    _scene->add<GameObject>(wrap, topFaceArgGameObject).addComponent<Model>(blackPlatformArg);
    _scene->add<GameObject>(wrap, forwardTopFaceArgGameObject).addComponent<Model>(blackPlatformArg);
    _scene->add<GameObject>(wrap, forwardGlassFaceArgGameObject).addComponent<Model>(glassPlatformArg);

    /*Create lots prefabs*/
    GameObjectCreateArg lot1GameObjectArg{"Lot1"};
    lot1GameObjectArg.transformArg.scale /= 2.f;

    GameObject& lot1GO = _scene->add<GameObject>(_scene->getWorld(), lot1GameObjectArg);

    lot1GO.addComponent<Model>(greenPlatformArg);
    lot1GO.addComponent<PhysicalObject>().setMass(1);
    lot1GO.addComponent<SphereCollider>().setBounciness(0.4f);
    lot1GO.addComponent<Loot>();

    Save::createPrefab(lot1GO, "Lot1");
    lot1GO.destroy();

    GameObjectCreateArg lot2GameObjectArg{"Lot2"};
    lot2GameObjectArg.transformArg.scale /= 2.f;

    GameObject& lot2GO = _scene->add<GameObject>(_scene->getWorld(), lot2GameObjectArg);

    ModelCreateArg modelCrateArg{&ressourceManager.get<Shader>("LightAndTexture"),
                          &ressourceManager.get<std::vector<Material>>("CrateMaterial"),
                          &ressourceManager.get<Mesh>("Cube"),
                          "LightAndTexture",
                          "CrateMaterial",
                          "Cube"};

    lot2GO.addComponent<Model>(modelCrateArg);
    lot2GO.addComponent<PhysicalObject>().setMass(1);
    lot2GO.addComponent<SphereCollider>().setBounciness(0.4f);
    lot2GO.addComponent<Loot>();

    Save::createPrefab(lot2GO, "Lot2");
    lot2GO.destroy();

    /*Create spawner*/
    GameObject& lotsContener = _scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"LotsContener"});

    GameObjectCreateArg spawnerGOArg;
    {
        spawnerGOArg.name = "Spawner";
        spawnerGOArg.transformArg.position = {wrapWidth / 4.f, wrapHeight / 2.f, 0.f};

        GameObject& spawnerGO = _scene->add<GameObject>(lootMachin, spawnerGOArg);
        spawnerGO.addComponent<CircularEntitiesSpawner>(&lotsContener, 0.1f, 0.5f, 0.f);

        //Save::createPrefab(spawnerGO, spawnerGOArg.name);
        //spawnerGO.destroy();
    }
}

void Demo::loadCamera()
{
    CameraPerspectiveCreateArg camArg{{0.f, 0.f, 30.f}, {0.f, 0.f, 0.f}, _gameEngine.getWinSize().width / static_cast<float>(_gameEngine.getWinSize().heigth), 0.1f, 10000.0f, 45.0f, "MainCamera"};
    mainCamera = &_scene->add<Camera>(_scene->getWorld(), camArg);
    dynamic_cast<Camera *>(mainCamera)->use();
}

void Demo::loadEntity(t_RessourcesManager &ressourceManager)
{
    loadTimeManager             ();
    loadRock                    (ressourceManager, 100);
    loadTree                    (ressourceManager, 50);
    loadSkybox                  (ressourceManager);
    loadPlayer                  (ressourceManager);
    loadGround                  (ressourceManager);
    loadFog                     (ressourceManager, 20);
    //loadTower                  (ressourceManager);Game
    loadLootMachin               (ressourceManager);
}
void Demo::loadLights(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg lightSphereGameObjectArg{"Sun",
                                                 {{0.f, 0.f, 0.f},
                                                  {0.f, 0.f, 0.f},
                                                  {1.f, 1.f, 1.f}}};

    ModelCreateArg lightSphereArg{&ressourceManager.get<Shader>("ColorWithLight"),
                                  &ressourceManager.get<std::vector<Material>>("DefaultMaterial"),
                                  &ressourceManager.get<Mesh>("Sphere"),
                                  "ColorWithLight",
                                  "DefaultMaterial",
                                  "Sphere",
                                  true};

    DirectionnalLightCreateArg lightArg2{{0.f, 1.f, -1.f},
                                         {1.f, 1.f, 1.f, 0.1f},
                                         {1.f, 1.f, 1.f, 0.7f},
                                         {1.f, 1.f, 1.f, 1.f},
                                         true};

    GameObjectCreateArg pointLightGameObjectArg{"PointLight",
                                                {{5.f, 10.f, -5.f},
                                                 {0.f, 0.f, 0.f},
                                                 {1.f, 1.f, 1.f}}};

    PointLightCreateArg lightArg5{{1.f, 1.f, 1.f, 0.f},
                                  {0.f, 1.f, 0.f, 0.7f},
                                  {1.f, 1.f, 1.f, 0.3f},
                                  0.f,
                                  0.05f,
                                  0.f,
                                  false};

    GameObjectCreateArg spotLightGameObjectArg{"SpotLight",
                                               {{5.f, 10.f, -5.f},
                                                {0.f, 0.f, 0.f},
                                                {1.f, 1.f, 1.f}}};

    SpotLightCreateArg lightArg6{{1.f, 1.f, 1.f, 0.f},
                                 {0.f, 1.f, 0.f, 0.7f},
                                 {1.f, 1.f, 1.f, 0.3f},
                                 0.f, 0.05f, 0.f,
                                 20.f, 0.5f, 
                                 false};

    GameObject &pl = _scene->add<GameObject>(_scene->getWorld(), lightSphereGameObjectArg);
    pl.addComponent<Model>(lightSphereArg);
    pl.addComponent<DirectionnalLight>(lightArg2);

    GameObject &pl1 = _scene->add<GameObject>(_scene->getWorld(), pointLightGameObjectArg);
    pl1.addComponent<Model>(lightSphereArg);
    pl1.addComponent<PointLight>(lightArg5);

    GameObject &pl2 = _scene->add<GameObject>(_scene->getWorld(), spotLightGameObjectArg);
    pl2.addComponent<Model>(lightSphereArg);
    pl2.addComponent<SpotLight>(lightArg6);
}

void Demo::loadReferential(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg refGO{"Right ref",
                              {{100.f, 0.f, 0.f},
                               {0.f, 0.f, 0.f},
                               {5.f, 5.f, 5.f}}};

    ModelCreateArg sphereModel{&ressourceManager.get<Shader>("ColorWithLight"),
                               &ressourceManager.get<std::vector<Material>>("RedMaterial"),
                               &ressourceManager.get<Mesh>("Sphere"),
                               "ColorWithLight",
                               "RedMaterial",
                               "Sphere",
                               true};

    _scene->add<GameObject>(_scene->getWorld(), refGO).addComponent<Model>(sphereModel);

    refGO.name = "Forward ref";
    refGO.transformArg.position = {0.f, 0.f, 100.f};
    sphereModel.pMaterials = &ressourceManager.get<std::vector<Material>>("BlueMaterial");
    _scene->add<GameObject>(_scene->getWorld(), refGO).addComponent<Model>(sphereModel);

    refGO.name = "Up ref";
    refGO.transformArg.position = {0.f, 100.f, 0.f};
    sphereModel.pMaterials = &ressourceManager.get<std::vector<Material>>("GreenMaterial");
    _scene->add<GameObject>(_scene->getWorld(), refGO).addComponent<Model>(sphereModel);
}

void Demo::loadUI(t_RessourcesManager &ressourceManager)
{
    FontCreateArg fontarg{"./ressources/font/opensans.ttf", 40};
    Font *pfont = &ressourceManager.add<Font>("font1", fontarg);
    FontCreateArg fontarg2{"./ressources/font/opensans.ttf", 25};
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
                                tempX - 155, tempY - 400,
                                175.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Forward :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionForwardButton", pfont, buttonShader,
                                 tempX + 50, tempY - 400,
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
                                tempX - 185, tempY - 300,
                                200.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Backward :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionBackwardButton", pfont, buttonShader,
                                 tempX + 50, tempY - 300,
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
                                tempX - 75, tempY - 200,
                                150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Left :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionLeftButton", pfont, buttonShader,
                                 tempX + 50, tempY - 200,
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
                                tempX - 100, tempY - 100,
                                150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Right :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionRightButton", pfont, buttonShader,
                                 tempX + 50, tempY - 100,
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

    ressourceManager.add<Title>("OptionSwitchLightStateTitle", pfont, buttonShader,
                                tempX - 310, tempY,
                                400.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Switch light state :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionSwitchLightStateButton", pfont, buttonShader,
                                 tempX + 50, tempY,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.switchFlashLightState)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.switchFlashLightState = key;
            ressourceManager.get<Button>("OptionSwitchLightStateButton").value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionSwitchLightStateButton").updateTexture();
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

    ressourceManager.add<Button>("OptionSwitchButton", pfont, buttonShader,
                                 tempX - 45, tempY + 200,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "WASD",
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        Button *switchButton = &ressourceManager.get<Button>("OptionSwitchButton");
        Button *upButton = &ressourceManager.get<Button>("OptionForwardButton");
        Button *downButton = &ressourceManager.get<Button>("OptionBackwardButton");
        Button *rightButton = &ressourceManager.get<Button>("OptionRightButton");
        Button *leftButton = &ressourceManager.get<Button>("OptionLeftButton");
        if (switchButton->value.compare("WASD") == 0)
        {
            Input::keyboard.up = SDL_SCANCODE_Z;
            Input::keyboard.down = SDL_SCANCODE_S;
            Input::keyboard.right = SDL_SCANCODE_D;
            Input::keyboard.left = SDL_SCANCODE_Q;
            upButton->value = "Z";
            downButton->value = "S";
            rightButton->value = "D";
            leftButton->value = "Q";
            switchButton->value = "ZQSD";
        }
        else if (switchButton->value.compare("ZQSD") == 0)
        {
            Input::keyboard.up = SDL_SCANCODE_W;
            Input::keyboard.down = SDL_SCANCODE_S;
            Input::keyboard.right = SDL_SCANCODE_D;
            Input::keyboard.left = SDL_SCANCODE_A;
            upButton->value = "W";
            downButton->value = "S";
            rightButton->value = "D";
            leftButton->value = "A";
            switchButton->value = "WASD";
        }
        switchButton->updateTexture();
        upButton->updateTexture();
        downButton->updateTexture();
        rightButton->updateTexture();
        leftButton->updateTexture();
    };

    ressourceManager.add<Button>("Return", pfont, buttonShader,
                                 tempX - 50, tempY + 300,
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
        std::cout << saves<< std::endl;
    }

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
            setupScene(*_scene, saves.c_str());
            mainCamera = &_scene->getGameObject("world/MainCamera");

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
        _gameEngine.gameState = E_GAME_STATE::STARTING;
    };

#pragma endregion

#pragma region UI in Game

ressourceManager.add<ReferencedTitle>("WaveIndicatorUI", pfont, buttonShader,
                                tempX  - tempX / 10.f, tempY - tempY / 10.f * 9.f,
                                150.0f, 60.0f, SDL_Color{200, 30, 30, 255}, (int*)_scene->getGameObject("waveManager").getComponent<WaveManager>()->getPCurrentWave(),"Wave ", E_GAME_STATE::RUNNING);

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

    ressourceManager.add<Image>("CrosshairImage",
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
    GameObjectCreateArg NexusGameObjectArg{"Nexus"};

    ModelCreateArg modelNexusArg{&ressourceManager.get<Shader>("LightAndTexture"),
                          &ressourceManager.get<std::vector<Material>>("NexusMaterial"),
                          &ressourceManager.get<Mesh>("NexusMesh"),
                          "LightAndTexture",
                          "NexusMaterial",
                          "NexusMesh"};
    modelNexusArg.isOpaque = false;

    PointLightCreateArg lightArgNexusLight{{0.f, 0.2f, 1.f, 0.2f},
                                {0.f, 0.2f, 1.f, 0.5f},
                                {0.f, 0.2f, 1.f, 0.7f},
                                0.f, 0.05f, 0.f, true};

    GameObject& nexus = _scene->add<GameObject>(_scene->getWorld(), NexusGameObjectArg);
    nexus.setTranslation(Vec3{-10, -15, -10});
    nexus.setScale(Vec3{0.3f, 0.3f, 0.3f});
    nexus.addComponent<Model>(modelNexusArg);
    nexus.addComponent<Nexus>();
    nexus.addComponent<LevitationMovement>(1.f, 1.f);
    nexus.addComponent<PointLight>(lightArgNexusLight);
    

    GameObject* checkpoint1 = &_scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg {"checkpoint1"});
    checkpoint1->addComponent<Checkpoint>().addCheckpoint(Vec3{10, -15, 10});
    checkpoint1->getComponent<Checkpoint>()->addCheckpoint(Vec3{-10, -15, -10});

    enemiesContener = &_scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"EnemiesContener"});

    GameObjectCreateArg Ennemy1GameObjectArg{"Ennemy"};

    ModelCreateArg modelArg{&ressourceManager.get<Shader>("ColorWithLight"),
                        &ressourceManager.get<std::vector<Material>>("GreenMaterial"),
                        &ressourceManager.get<Mesh>("Cube"),
                        "ColorWithLight",
                        "GreenMaterial",
                        "Cube"};

        GameObject& enemy1 = _scene->add<GameObject>(_scene->getWorld(), Ennemy1GameObjectArg);

        enemy1.setScale(Vec3{3.f, 5.f, 3.f});

        enemy1.addComponent<Model>(modelArg);
        enemy1.addComponent<PhysicalObject>().setMass(1);
        enemy1.addComponent<SphereCollider>().setBounciness(0.f);
        enemy1.getComponent<SphereCollider>()->setFriction(0.97f);

        enemy1.addComponent<EnnemyController>(  &Scene::getCurrentScene()->getGameObject("world/Players/Player1"), 
                                                &Scene::getCurrentScene()->getGameObject("world/Nexus"), 
                                                checkpoint1->getComponent<Checkpoint>());

    Save::createPrefab(enemy1, "enemy1");
    enemy1.destroy();

    GameObjectCreateArg CrateGameObjectArg{"Crate"};

    ModelCreateArg modelCrateArg{&ressourceManager.get<Shader>("LightAndTexture"),
                                 &ressourceManager.get<std::vector<Material>>("CrateMaterial"),
                                 &ressourceManager.get<Mesh>("Cube"),
                                 "LightAndTexture",
                                 "CrateMaterial",
                                 "Cube"};

    GameObject& crate = _scene->add<GameObject>(_scene->getWorld(), CrateGameObjectArg);

    crate.addComponent<Model>(modelCrateArg);
    PhysicalObject &physicalObjectComp = crate.addComponent<PhysicalObject>();
    physicalObjectComp.setMass(3);
    crate.addComponent<PushedOnShoot>();
    crate.addComponent<SphereCollider>().setBounciness(0.f);
    crate.getComponent<SphereCollider>()->setFriction(0.97f);
    Save::createPrefab(crate, "Crate");
    crate.destroy();

    _scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"DecalContenor", {{0.f, 0.f, 0.f}}}).addComponent<MaxElementConteneur>(10);

    /*Create spawner*/

    GameObjectCreateArg spawnerGOArg;

    GameObjectCreateArg pointLightGameObjectArg{"PointLight", {{0.f, 2.f, 0.f}}};
    PointLightCreateArg lightArg{{1.f, 1.f, 1.f, 0.f},
                                {1.f, 0.f, 0.f, 0.7f},
                                {1.f, 1.f, 1.f, 0.3f},
                                0.f, 0.05f, 0.f, true};
    {
        spawnerGOArg.name = "Spawner1";
        spawnerGOArg.transformArg.position = {5.f, 5.f, 5.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        spawnerGO.addComponent<CircularEntitiesSpawner>(enemiesContener, 2.f, 0.5f, 0.f);
        _scene->add<GameObject>(spawnerGO, pointLightGameObjectArg).addComponent<PointLight>(lightArg);

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroy();
    }

    {
        spawnerGOArg.name = "Spawner2";
        spawnerGOArg.transformArg.position = {-5.f, 5.f, 5.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        spawnerGO.addComponent<CircularEntitiesSpawner>(enemiesContener, 2.f, 0.5f, 0.f);
        _scene->add<GameObject>(spawnerGO, pointLightGameObjectArg).addComponent<PointLight>(lightArg);

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroy();
    }

    {
        spawnerGOArg.name = "Spawner3";
        spawnerGOArg.transformArg.position = {5.f, 5.f, -5.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        spawnerGO.addComponent<CircularEntitiesSpawner>(enemiesContener, 2.f, 0.5f, 0.f);
        _scene->add<GameObject>(spawnerGO, pointLightGameObjectArg).addComponent<PointLight>(lightArg);

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroy();
    }

    {
        spawnerGOArg.name = "Spawner4";
        spawnerGOArg.transformArg.position = {-5.f, 5.f, -5.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        spawnerGO.addComponent<CircularEntitiesSpawner>(enemiesContener, 2.f, 0.5f, 0.f);
        _scene->add<GameObject>(spawnerGO, pointLightGameObjectArg).addComponent<PointLight>(lightArg);

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroy();
    }

    /*Create wave manager and assign spawner and enemies prefabs*/
    GameObjectCreateArg waveManagerArg {"waveManager"};

    GameObject& waveManagerGO = _scene->add<GameObject>(_scene->getWorld(), waveManagerArg);
    SpawnerPrefabs spawnerPrefs {"Spawner1", "Spawner2", "Spawner3", "Spawner4"};
    EnemiesPrefabs enemiesPrefs {"Crate", "enemy1"};
    waveManagerGO.addComponent<WaveManager>(spawnerPrefs, enemiesPrefs, 0, 0).nextWave();
}

void Demo::loadTimeManager        ()
{
    GameObject& timeManager = _scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"TimeManager"});
    timeManager.addComponent<DayNightCycle>(60.f, 120.f, 30.f, 180.f, 120.f, 30.f, 100.f, 1000.f);
}

void Demo::updateControl()
{
    /*
    float dist = 5.f;
    _scene->getGameObject("Z").setTranslation(_scene->getGameObject("Player").getPosition() + dist * _scene->getGameObject("Player").getVecForward());
    _scene->getGameObject("Y").setTranslation(_scene->getGameObject("Player").getPosition() + dist * _scene->getGameObject("Player").getVecUp());
    _scene->getGameObject("X").setTranslation(_scene->getGameObject("Player").getPosition() + dist * _scene->getGameObject("Player").getVecRight());
    */
    if (Input::keyboard.getKeyState(SDL_SCANCODE_F1) == E_KEY_STATE::TOUCHED)
    {
        Editor::_enable = !Editor::_enable;
        usingMouse = Editor::_enable;
    }
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
}