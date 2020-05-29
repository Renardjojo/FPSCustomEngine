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
#include "GE/Physics/ColliderShape/SphereCollider.hpp"
#include "GE/Physics/ColliderShape/OrientedBoxCollider.hpp"
#include "GE/Physics/ColliderShape/CapsuleCollider.hpp"
#include "GE/Physics/ColliderShape/AABBCollider.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Ressources/Saves.hpp"

#include "Game/define.h"
#include "Game/BarIndicatorController.hpp"
#include "Game/CircularEntitiesSpawner.hpp"
#include "Game/ParticuleGenerator.hpp"
#include "Game/MaxElementConteneur.hpp"
#include "Game/PushedOnShoot.hpp"
#include "Game/GroundController.hpp"
#include "Game/WaveManager.hpp"

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
    loadCamera();
    loadEntity(_gameEngine.ressourceManager_);
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
}

void Demo::update() noexcept
{
    SDL_ShowCursor(usingMouse);
    SDL_CaptureMouse(usingMouse? SDL_FALSE :SDL_TRUE);
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
    ressourceManager.add<Shader>("Color", "./ressources/shader/vCloud.vs", "./ressources/shader/fColorOnly.fs", AMBIANTE_COLOR_ONLY);
    ressourceManager.add<Shader>("TextureOnly", "./ressources/shader/vCloud.vs", "./ressources/shader/fTextureOnly.fs");
    ressourceManager.add<Shader>("LightAndTexture", "./ressources/shader/vTexture2.vs", "./ressources/shader/fTexture2.fs", LIGHT_BLIN_PHONG);

    MaterialAndTextureCreateArg matDefault;
    matDefault.name_ = "DefaultMaterial";
    matDefault.pathDiffuseTexture = nullptr;
    matDefault.flipTexture = false;

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name_, std::move(material));
    }

    matDefault.name_ = "PinkMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{1.f, 0.f, 1.f, 1.f};

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name_, std::move(material));
    }

    matDefault.name_ = "RedMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{1.f, 0.f, 0.f, 1.f};
    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name_, std::move(material));
    }

    matDefault.name_ = "GreenMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{0.f, 1.f, 0.f, 1.f};

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name_, std::move(material));
    }

    matDefault.name_ = "BlueMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{0.f, 0.f, 1.f, 1.f};

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name_, std::move(material));
    }

    matDefault.name_ = "BlackMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{0.f, 0.f, 0.f, 1.f};

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name_, std::move(material));
    }

    matDefault.name_ = "BrownMaterial";
    matDefault.comp_.ambient.rgbi = Vec4{0.45f, 0.25f, 0.04f, 1.f};

    {
        std::vector<Material> material;
        material.emplace_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name_, std::move(material));
    }

    ressourceManager.add<Mesh>("Cube", Mesh::createCube(1));
    ressourceManager.add<Mesh>("Sphere", Mesh::createSphere(25, 25));
    ressourceManager.add<Mesh>("Plane", Mesh::createPlane());
    ressourceManager.add<Mesh>("PlaneZ", Mesh::createPlane(1.f, 0, 0, Mesh::Axis::Z));

    MaterialAndTextureCreateArg matBulletHole;
    matBulletHole.name_                = "BulletHole";
    matBulletHole.pathDiffuseTexture   = "./ressources/texture/bulletHole.png";

    {
        std::vector<Material> material;
        material.emplace_back(matBulletHole);
        ressourceManager.add<std::vector<Material>>(matBulletHole.name_, std::move(material));
    }

    //loadRockRessource          (ressourceManager);
    //loadTreeRessource          (ressourceManager);
    loadSkyboxRessource        (ressourceManager);
    loadGunRessource           (ressourceManager);
    loadPseudoRessource        (ressourceManager);
    loadPlayerRessource        (ressourceManager);
    //loadSpotLightRessource     (ressourceManager);
    //loadTowerRessource         (ressourceManager);
    loadGroundRessource        (ressourceManager);
    loadCrateRessource          (ressourceManager);  
}

void Demo::loadRockRessource          (t_RessourcesManager& ressourceManager)
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
        material.assign (materialAttribs.begin(),materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("RockMaterials", std::move(material));
    }
}

void Demo::loadTreeRessource          (t_RessourcesManager& ressourceManager)
{
    Attrib                      attrib;
    std::vector<Shape>          shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./ressources/obj/Tree.obj", &attrib, &shape, &materialAttribs);

    ressourceManager.add<Mesh>("TreeMesh", attrib, shape);

    {
        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign (materialAttribs.begin(),materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("TreeMaterials", std::move(material));
    }
}

void Demo::loadSkyboxRessource        (t_RessourcesManager& ressourceManager)
{
    MaterialAndTextureCreateArg matSkybox;
    matSkybox.name_ = "SkyBox";
    matSkybox.pathDiffuseTexture = "./ressources/texture/arrakisday/skb.bmp";
    matSkybox.flipTexture = false;
    matSkybox.filterType = E_FilterType::LINEAR;

    ressourceManager.add<Shader>("SkyboxShader", "./ressources/shader/vSkybox.vs", "./ressources/shader/fLightingSkybox.fs", SKYBOX | LIGHT_BLIN_PHONG);
    ressourceManager.add<std::vector<Material>>("SkyboxMaterial", std::vector<Material>{matSkybox});
    ressourceManager.add<Mesh>("SkyboxMesh", "./ressources/obj/skybox.obj");
}

void Demo::loadGunRessource           (t_RessourcesManager& ressourceManager)
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

void Demo::loadPseudoRessource        (t_RessourcesManager& ressourceManager)
{
    FontCreateArg fontArg;
    fontArg.fontRoot    = "./ressources/font/COURIER.ttf";
    fontArg.fontSize    = 25;

    Font& font  = ressourceManager.add<Font>("COURIER", fontArg);

    TextCreateArg textArg;
    textArg.text = "Mjr. Cavalsky";
    textArg.pFont = &font;
    textArg.color = {0.5f, 0.5f, 0.f, 1.f};

    MaterialCreateArg matText;
    matText.name                = "PseudoMaterial";
    matText.pTexture            = std::make_unique<Text>(textArg);
    matText.comp.ambient.ki     = 1.f;
    matText.comp.diffuse.ki     = 0.f;
    matText.comp.specular.ki    = 0.f;

    {
        std::vector<Material> material;
        material.emplace_back(matText);
        ressourceManager.add<std::vector<Material>>(matText.name, std::move(material));
    }
}


void Demo::loadPlayerRessource        (t_RessourcesManager& ressourceManager)
{
    Attrib                      attrib;
    std::vector<Shape>          shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./ressources/obj/CODSoldier.obj", &attrib, &shape, &materialAttribs);

    ressourceManager.add<Mesh>("Soldier1Mesh", attrib, shape);

    {
        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign (materialAttribs.begin(),materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("Soldier1Materials", std::move(material));
    } 
}

void Demo::loadTowerRessource         (t_RessourcesManager& ressourceManager)
{
    Attrib                      attrib;
    std::vector<Shape>          shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./ressources/obj/guardTower.obj", &attrib, &shape, &materialAttribs);

    ressourceManager.add<Mesh>("GuardTowerMesh", attrib, shape);

    {
        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign (materialAttribs.begin(),materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("GuardTowerMaterials", std::move(material));
    }
}

void Demo::loadSpotLightRessource     (t_RessourcesManager& ressourceManager)
{
    Attrib                      attrib;
    std::vector<Shape>          shape;
    std::vector<MaterialAttrib> materialAttribs;

    loadObjWithMTL("./ressources/obj/spotLight.obj", &attrib, &shape, &materialAttribs);

    ressourceManager.add<Mesh>("SpotLightMesh", attrib, shape);

    {
        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign (materialAttribs.begin(),materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("SpotLightMaterial", std::move(material));
    }
}


void Demo::loadGroundRessource        (t_RessourcesManager& ressourceManager)
{
    ressourceManager.add<Mesh>("GroundMesh" ,Mesh::createPlane(1000));

    MaterialAndTextureCreateArg matGround;
    matGround.name_ = "Ground";
    matGround.comp_.shininess = 1.f;
    matGround.comp_.specular.rgbi = {1.f, 1.f, 1.f, 0.f};
    matGround.pathDiffuseTexture = "./ressources/texture/arrakisday/arrakisday_dn.tga";
    matGround.wrapType = E_WrapType::MIRRORED_REPEAT;

    {
        std::vector<Material> material;
        material.emplace_back(matGround);
        ressourceManager.add<std::vector<Material>>(matGround.name_, std::move(material));
    }
}   

void Demo::loadCrateRessource          (t_RessourcesManager& ressourceManager)
{
    MaterialAndTextureCreateArg matCrate;
    matCrate.name_ = "CrateMaterial";
    matCrate.comp_.specular.rgbi = {1.f, 1.f, 1.f, 0.1f};
    matCrate.pathDiffuseTexture = "./ressources/texture/crate.png";

    {
        std::vector<Material> material;
        material.emplace_back(matCrate);
        ressourceManager.add<std::vector<Material>>(matCrate.name_, std::move(material));
    }
}

void Demo::loadRock                   (t_RessourcesManager& ressourceManager, unsigned int number)
{
    GameObjectCreateArg rockGameObject  {"Rocks",
                                       {{0.f, 0.f, 0.f},
                                        {0.f, 0.f, 0.f},
                                        {1.f, 1.f, 1.f}}};

    std::vector<Material>& vecMaterials = ressourceManager.get<std::vector<Material>>("RockMaterials");
    
    ModelCreateArg rockModelArg{&ressourceManager.get<Shader>("LightAndTexture"),
                            &vecMaterials,
                            &ressourceManager.get<Mesh>("RockMesh"),
                            "LightAndTexture",
                            "RockMaterials",
                            "RockMesh"};
    
    GameObject& rockContener = _scene->add<GameObject>(_scene->getWorld(), rockGameObject);

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

void Demo::loadTree                   (t_RessourcesManager& ressourceManager, unsigned int number)
{
    GameObjectCreateArg treeGameObject  {"Trees",
                                       {{0.f, 0.f, 0.f},
                                        {0.f, 0.f, 0.f},
                                        {1.f, 1.f, 1.f}}};

    GameObject& treeContener = _scene->add<GameObject>(_scene->getWorld(), treeGameObject);

    std::vector<Material>& vecMaterials = ressourceManager.get<std::vector<Material>>("TreeMaterials");
    
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

void Demo::loadSkybox                 (t_RessourcesManager& ressourceManager)
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

void Demo::loadPlayer                 (t_RessourcesManager& ressourceManager)
{
    GameObjectCreateArg playerGameObject    {"Players",
                                            {{0.f, 0.f, 0.f},
                                            {0.f, 0.f, 0.f},
                                            {1.f, 1.f, 1.f}}};

    GameObject& playerContener = _scene->add<GameObject>(_scene->getWorld(), playerGameObject);

    std::vector<Material>& vecMaterials = ressourceManager.get<std::vector<Material>>("Soldier1Materials");

    playerGameObject.name = "Player1";
    playerGameObject.transformArg.position = {2.f, 10.f, 2.f};
    GameObject& player1GO = _scene->add<GameObject>(playerContener, playerGameObject);
    player1GO.addComponent<PlayerController>();
    player1GO.addComponent<PhysicalObject>().setMass(1);
    player1GO.addComponent<SphereCollider>().getLocalSphere().setRadius(5.f);
    player1GO.getComponent<SphereCollider>()->setBounciness(0.f);

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

    //load guns
    GameObjectCreateArg sniperGameObject    {"Sniper",
                                            {{-0.5f, -1.5f, 3.2f},
                                            {0.f, -M_PI_2, 0.f},
                                            {0.02f, 0.02f, 0.02f}}};

    std::vector<Material>& vecMaterialsGun = ressourceManager.get<std::vector<Material>>("SniperMaterials");

    ModelCreateArg sniperModelArg{&ressourceManager.get<Shader>("LightAndTexture"),
                                    &vecMaterialsGun,
                                    &ressourceManager.get<Mesh>("SniperMesh"),
                                    "LightAndTexture",
                                    "SniperMaterials",
                                    "SniperMesh"};

    _scene->add<GameObject>(player1GO, sniperGameObject).addComponent<Model>(sniperModelArg);

    //load billboards
    std::vector<Material>& vecMaterialsPseudo = ressourceManager.get<std::vector<Material>>("PseudoMaterial");
    Size textureSize = vecMaterialsPseudo[0].getPDiffuseTexture()->getSize();

    GameObjectCreateArg pseudoGameObject    {"Pseudo",
                                            {{0.0f, 5.0f, 0.0f},
                                            {0.f, 0.f, 0.f},
                                            {-textureSize.width / 10.f, 1.f, textureSize.heigth / 10.f}}};

    ModelCreateArg planeArg    { &ressourceManager.get<Shader>("TextureOnly"), 
                                 &vecMaterialsPseudo,
                                 &ressourceManager.get<Mesh>("Plane"),
                                "TextureOnly",
                                "PseudoMaterial",
                                "Plane", true, false};

    _scene->add<GameObject>(player1GO, pseudoGameObject).addComponent<BillBoard>(planeArg);
}

void Demo::loadTower                  (t_RessourcesManager& ressourceManager)
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
    GameObjectCreateArg spotLightArgGameObject  {"SpotLight",
                                                {{0.0f, 48.0f, -5.0f},
                                                {M_PI, 0.f, 0.f},
                                                {0.05f, 0.05f, 0.05f}}};

    ModelCreateArg spotLightModelArg    {&ressourceManager.get<Shader>("LightAndTexture"),
                                        &ressourceManager.get<std::vector<Material>>("SpotLightMaterial"),
                                        &ressourceManager.get<Mesh>("SpotLightMesh"),
                                        "LightAndTexture",
                                        "SpotLightMaterial",
                                        "SpotLightMesh"};

    GameObject& spotLightGO = _scene->add<GameObject>(towerGO, spotLightArgGameObject);
    spotLightGO.addComponent<Model>(spotLightModelArg);

    //create light
    GameObjectCreateArg lightArgGameObject      {"light",
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
                                     {0.f, -0.5f, 0.6f},
                                     5.f, 7.f};

    GameObject& lightGO = _scene->add<GameObject>(spotLightGO, lightArgGameObject);
    lightGO.addComponent<Model>(spherModelarg);
    lightGO.addComponent<SpotLight>(lightSpotArg).enable(true);
}

void Demo::loadGround                 (t_RessourcesManager& ressourceManager)
{
    GameObjectCreateArg groundArgGameObject{"Ground",
                                            {{0.f, -1.f, 0.f},
                                             {0.f, 0.f, 0.f},
                                             {100000.f, 1.f, 100000.f}}};

    ModelCreateArg groundArg{&ressourceManager.get<Shader>("LightAndTexture"),
                             &ressourceManager.get<std::vector<Material>>("Ground"),
                             &ressourceManager.get<Mesh>("GroundMesh"),
                             "TextureOnly",
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

void Demo::loadCamera()
{
    CameraPerspectiveCreateArg camArg{{0.f, 0.f, 30.f}, {0.f, 0.f, 0.f}, _gameEngine.getWinSize().width / static_cast<float>(_gameEngine.getWinSize().heigth), 0.1f, 10000.0f, 45.0f, "MainCamera"};
    mainCamera = &_scene->add<Camera>(_scene->getWorld(), camArg);
    dynamic_cast<Camera *>(mainCamera)->use();
}

void Demo::loadEntity(t_RessourcesManager &ressourceManager)
{
/*    GameObjectCreateArg cubeGameObject{"cube1",
                                       {{-0.7f, -5.f, 0.f},
                                        {0.f, 0.f, 45.f},
                                        {5.f, 1.f, 5.f}}};

    ModelCreateArg cube1arg{&ressourceManager.get<Shader>("ColorWithLight"),
                            &ressourceManager.get<std::vector<Material>>("PinkMaterial"),
                            &ressourceManager.get<Mesh>("Cube"),
                            "ColorWithLight",
                            "PinkMaterial",
                            "Cube"};

    _scene->add<GameObject>(_scene->getWorld(), cubeGameObject).addComponent<Model>(cube1arg);
    _scene->getGameObject("world/cube1").addComponent<OrientedBoxCollider>();

    GameObjectCreateArg cube2GameObject{"cube2",
                                        {{-5.f, -10.f, 0.f},
                                         {0.f, 0.f, -45.f},
                                         {5.f, 1.f, 5.f}}};

    ModelCreateArg cube2arg{&ressourceManager.get<Shader>("ColorWithLight"),
                            &ressourceManager.get<std::vector<Material>>("DefaultMaterial"),
                            &ressourceManager.get<Mesh>("Cube"),
                            "ColorWithLight",
                            "DefaultMaterial",
                            "Cube"};

    _scene->add<GameObject>(_scene->getWorld(), cube2GameObject).addComponent<Model>(cube2arg);
    _scene->getGameObject("world/cube2").addComponent<OrientedBoxCollider>();

    GameObjectCreateArg cube3GameObject{"cube3",
                                        {{0.f, -11.f, 0.f},
                                         {0.f, 0.f, 45.f},
                                         {5.f, 1.f, 5.f}}};

    ModelCreateArg cube3arg{&ressourceManager.get<Shader>("ColorWithLight"),
                            &ressourceManager.get<std::vector<Material>>("DefaultMaterial"),
                            &ressourceManager.get<Mesh>("Cube"),
                            "ColorWithLight",
                            "DefaultMaterial",
                            "Cube"};

    _scene->add<GameObject>(_scene->getWorld(), cube3GameObject).addComponent<Model>(cube3arg);
    _scene->getGameObject("world/cube3").addComponent<OrientedBoxCollider>();*/
/*
    GameObjectCreateArg playerGameObject{"Player",
                                         {{-2.f, 5.f, 0.f},
                                          {0.f, 0.f, 0.f},
                                          {1.0f, 1.0f, 1.0f}}};

    ModelCreateArg playerModel{&ressourceManager.get<Shader>("ColorWithLight"),
                               &ressourceManager.get<std::vector<Material>>("PinkMaterial"),
                               &ressourceManager.get<Mesh>("Sphere"),
                               "ColorWithLight",
                               "PinkMaterial",
                               "Sphere"};

    GameObject &player = _scene->add<GameObject>(_scene->getWorld(), playerGameObject);
    player.addComponent<Model>(playerModel);

    GameObjectCreateArg ReticuleGameObject{"Z",
                                           {{0.f, 0.f, 0.f},
                                            {0.f, 0.f, 0.f},
                                            {0.2f, 0.2f, 0.2f}}};

    ModelCreateArg ReticuleModel{&ressourceManager.get<Shader>("ColorWithLight"),
                               &ressourceManager.get<std::vector<Material>>("BlueMaterial"),
                               &ressourceManager.get<Mesh>("Sphere"),
                               "ColorWithLight",
                               "RedMaterial",
                               "Sphere"};*/
/*
    GameObject &ReticuleX = _scene->add<GameObject>(_scene->getWorld(), ReticuleGameObject);
    ReticuleX.addComponent<Model>(ReticuleModel); 

    ReticuleGameObject.name = "Y";
    ReticuleModel.pMaterials = &ressourceManager.get<std::vector<Material>>("GreenMaterial");

    GameObject &ReticuleY = _scene->add<GameObject>(_scene->getWorld(), ReticuleGameObject);
    ReticuleY.addComponent<Model>(ReticuleModel); 

    ReticuleGameObject.name = "X";
    ReticuleModel.pMaterials = &ressourceManager.get<std::vector<Material>>("RedMaterial");

    GameObject &ReticuleZ = _scene->add<GameObject>(_scene->getWorld(), ReticuleGameObject);
    ReticuleZ.addComponent<Model>(ReticuleModel);*/

    /*Add life bar on player*//*
    GameObjectCreateArg lifeBarGameObject{"lifeBar",
                                          {{0.f, 2.f, 0.f},
                                           {0.f, 0.f, 0.f},
                                           {1.f, 0.3f, 0.1f}}};

    ModelCreateArg billBoardArg{&ressourceManager.get<Shader>("Color"),
                                &ressourceManager.get<std::vector<Material>>("GreenMaterial"),
                                &ressourceManager.get<Mesh>("Sphere"),
                                "Color",
                                "GreenMaterial",
                                "Sphere"};

    player.addComponent<PlayerController>();
    player.addComponent<PhysicalObject>();
    player.getComponent<PhysicalObject>()->setMass(1);
    player.addComponent<SphereCollider>();
    player.getComponent<SphereCollider>()->setBounciness(0.f);*/

    //loadRock                   (ressourceManager, 50);
    //loadTree                   (ressourceManager, 10);
    loadSkybox                 (ressourceManager);
    loadPlayer                 (ressourceManager);
    loadGround                 (ressourceManager);
    //loadTower                  (ressourceManager);Game
}
void Demo::loadLights(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg lightSphereGameObjectArg{"DirectionnalLight",
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
                                         {1.f, 1.f, 1.f, 1.f}, true};

    GameObjectCreateArg pointLightGameObjectArg{"PointLight",
                                                {{5.f, 10.f, -5.f},
                                                 {0.f, 0.f, 0.f},
                                                 {1.f, 1.f, 1.f}}};

    PointLightCreateArg lightArg5{{1.f, 1.f, 1.f, 0.f},
                                  {0.f, 1.f, 0.f, 0.7f},
                                  {1.f, 1.f, 1.f, 0.3f},
                                  0.f, 0.05f, 0.f, false};

    GameObjectCreateArg spotLightGameObjectArg{"SpotLight",
                                               {{5.f, 10.f, -5.f},
                                                {0.f, 0.f, 0.f},
                                                {1.f, 1.f, 1.f}}};

    SpotLightCreateArg lightArg6{{1.f, 1.f, 1.f, 0.f},
                                 {0.f, 1.f, 0.f, 0.7f},
                                 {1.f, 1.f, 1.f, 0.3f},
                                 0.f, 0.05f, 0.f,
                                 Vec3::down,
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
        .function = [&]() 
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

    ressourceManager.add<Button>("OptionSwitchButton", pfont, buttonShader,
                                 tempX - 45, tempY + 200,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "WASD",
                                 E_GAME_STATE::OPTION)
    .function = [&]() 
    {
        Button* switchButton = &ressourceManager.get<Button>("OptionSwitchButton");
        Button* upButton = &ressourceManager.get<Button>("OptionForwardButton");
        Button* downButton = &ressourceManager.get<Button>("OptionBackwardButton");
        Button* rightButton = &ressourceManager.get<Button>("OptionRightButton");
        Button* leftButton = &ressourceManager.get<Button>("OptionLeftButton");
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
    /*Create enemies prefabs */
    GameObject* checkpoint1 = &_scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg {"checkpoint1"});
    checkpoint1->addComponent<Checkpoint>().addCheckpoint(Vec3{10, -10, 10});
    checkpoint1->getComponent<Checkpoint>()->addCheckpoint(Vec3{-10, -10, -10});

    enemiesContener = &_scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"EnemiesContener"});

    GameObjectCreateArg Ennemy1GameObjectArg{"Ennemy"};

    ModelCreateArg modelArg{&ressourceManager.get<Shader>("ColorWithLight"),
                        &ressourceManager.get<std::vector<Material>>("GreenMaterial"),
                        &ressourceManager.get<Mesh>("Cube"),
                        "ColorWithLight",
                        "GreenMaterial",
                        "Cube"};

        GameObject& enemy1 = _scene->add<GameObject>(_scene->getWorld(), Ennemy1GameObjectArg);

    enemy1.addComponent<Model>(modelArg);
    enemy1.addComponent<PhysicalObject>().setMass(1);
    enemy1.addComponent<SphereCollider>().setBounciness(0.4f);

    //enemy1.addComponent<EnnemyController>(&Scene::getCurrentScene()->getGameObject("world/Players/Player1"), checkpoint1->getComponent<Checkpoint>());

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
    PhysicalObject& physicalObjectComp = crate.addComponent<PhysicalObject>();
    physicalObjectComp.setMass(3);
    crate.addComponent<PushedOnShoot>();
    crate.addComponent<SphereCollider>().setBounciness(0.2f);

    Save::createPrefab(crate, "Crate");
    crate.destroy();

    _scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"DecalContenor", {{0.f, 0.f, 0.f}}}).addComponent<MaxElementConteneur>(10);

    /*Create spawner*/

    GameObjectCreateArg spawnerGOArg;

    {
        spawnerGOArg.name = "Spawner1";
        spawnerGOArg.transformArg.position = {5.f, 5.f, 5.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        spawnerGO.addComponent<CircularEntitiesSpawner>(enemiesContener, 2.f, 0.5f, 0.f);

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroy();
    }

    {
        spawnerGOArg.name = "Spawner2";
        spawnerGOArg.transformArg.position = {-5.f, 5.f, 5.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        spawnerGO.addComponent<CircularEntitiesSpawner>(enemiesContener, 2.f, 0.5f, 0.f);

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroy();
    }

    {
        spawnerGOArg.name = "Spawner3";
        spawnerGOArg.transformArg.position = {5.f, 5.f, -5.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        spawnerGO.addComponent<CircularEntitiesSpawner>(enemiesContener, 2.f, 0.5f, 0.f);

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroy();
    }

    {
        spawnerGOArg.name = "Spawner4";
        spawnerGOArg.transformArg.position = {-5.f, 5.f, -5.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        spawnerGO.addComponent<CircularEntitiesSpawner>(enemiesContener, 2.f, 0.5f, 0.f);

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