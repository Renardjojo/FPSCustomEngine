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
#include "Game/LifeLoot.hpp"
#include "Game/BombeLoot.hpp"
#include "Game/LevitationMovement.hpp"
#include "Game/UpgradeStation.hpp"

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
using namespace Save;
using namespace Engine::LowRenderer;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Parsers;
using namespace Engine::Core::System;
using namespace Engine::Core::DataStructure;
using namespace Engine::Core::InputSystem;

void Demo::loadEntity(t_RessourcesManager &ressourceManager)
{
    /*Fixe the seed to obtain a fixed procedural scene*/
    Random::initSeed(10.f);

    loadTimeManager             ();
    loadRock                    (ressourceManager, 100);
    loadTree                    (ressourceManager, 50);
    loadSkybox                  (ressourceManager);
    loadPlayer                  (ressourceManager);
    loadGround                  (ressourceManager);
    loadFog                     (ressourceManager, 20);
    loadLootMachin               (ressourceManager);

    /*Add randome seed*/
    Random::initSeed();
}

void Demo::loadRock                   (t_RessourcesManager& ressourceManager, unsigned int number)
{
    GameObjectCreateArg rockGameObject{"Rocks",
                                       {{0.f, 0.f, 0.f},
                                        {0.f, 0.f, 0.f},
                                        {1.f, 1.f, 1.f}}};

    std::vector<Material>* vecMaterials = ressourceManager.get<std::vector<Material>>("RockMaterials");

    ModelCreateArg rockModelArg{ressourceManager.get<Shader>("LightAndTexture"),
                                vecMaterials,
                                ressourceManager.get<Mesh>("RockMesh"),
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

        GameObject& rockGO = _scene->add<GameObject>(rockContener, rockGameObject);
        rockGO.addComponent<Model>(rockModelArg);
        rockGO.addComponent<OrientedBoxCollider>().setBounciness(0.4f);
    }
}

void Demo::loadTree(t_RessourcesManager &ressourceManager, unsigned int number)
{
    GameObjectCreateArg treeGameObject{"Trees",
                                       {{0.f, 0.f, 0.f},
                                        {0.f, 0.f, 0.f},
                                        {1.f, 1.f, 1.f}}};

    GameObject& treeContener = _scene->add<GameObject>(_scene->getWorld(), treeGameObject);
    std::vector<Material>* vecMaterials = ressourceManager.get<std::vector<Material>>("TreeMaterials");
    ModelCreateArg treeModelArg{ressourceManager.get<Shader>("LightAndTexture"),
                                vecMaterials,
                                ressourceManager.get<Mesh>("TreeMesh"),
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

        treeGameObject.transformArg.scale = {Random::ranged<float>(4.f, 8.f), Random::ranged<float>(4.f, 8.f), Random::ranged<float>(4.f, 8.f)};

        GameObject& treeGO = _scene->add<GameObject>(treeContener, treeGameObject);
        treeGO.addComponent<Model>(treeModelArg);

        GameObjectCreateArg treeColliderArg{"TreeCollider",
                                       {{0.f, 0.f, 0.f},
                                        {0.f, 0.f, 0.f},
                                        treeGameObject.transformArg.scale}};
        treeColliderArg.transformArg.scale.y *= 10.f;  
        treeColliderArg.transformArg.scale.x *= 0.001f;  
        treeColliderArg.transformArg.scale.z *= 0.001f;          
        _scene->add<GameObject>(treeGO, treeColliderArg).addComponent<OrientedBoxCollider>().setBounciness(0.4f);
    }
}

void Demo::loadSkybox(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg skyboxArgGameObject{"Skybox",
                                            {{0.f, 0.f, 0.f},
                                             {0.f, 0.f, 0.f},
                                             {10.f, 10.f, 10.f}}};

    ModelCreateArg skyboxArg{ressourceManager.get<Shader>("SkyboxShader"),
                             ressourceManager.get<std::vector<Material>>("SkyboxMaterial"),
                             ressourceManager.get<Mesh>("SkyboxMesh"),
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

    playerGameObject.name = "Player1";
    playerGameObject.transformArg.position = {2.f, 10.f, 2.f};
    GameObject& player1GO = _scene->add<GameObject>(playerContener, playerGameObject);
    PlayerController& playerControllerPlayer1 = player1GO.addComponent<PlayerController>();
    player1GO.addComponent<PhysicalObject>().setMass(1);
    player1GO.addComponent<SphereCollider>().getLocalSphere().setRadius(5.f);
    player1GO.getComponent<SphereCollider>()->setBounciness(0.f);
    player1GO.getComponent<SphereCollider>()->setFriction(0.95f);
    
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

        std::vector<Material>* vecMaterialsGun = ressourceManager.get<std::vector<Material>>("SniperMaterials");

        ModelCreateArg sniperModelArg{  ressourceManager.get<Shader>("LightAndTexture"),
                                        vecMaterialsGun,
                                        ressourceManager.get<Mesh>("SniperMesh"),
                                        "LightAndTexture",
                                        "SniperMaterials",
                                        "SniperMesh"};
            
        GameObject& sniperGO = _scene->add<GameObject>(player1GO, sniperGameObject);
        sniperGO.addComponent<Model>(sniperModelArg);
        Firearm& sniperComponent = sniperGO.addComponent<Sniper>(2.f, 1000.f, 1, 1.f, 5, 0.2f,ressourceManager.get<Sound>("Sniper"));
        playerControllerPlayer1.addFirearm(&sniperComponent);
    }
    //Shotgun
    {
        GameObjectCreateArg shotgunGameObject{"shotgun",{{-0.5f, -1.5f, 3.8f}, {0.f, M_PI, 0.f}}};
        std::vector<Material>* vecMaterialsShotgun = ressourceManager.get<std::vector<Material>>("ShotgunMaterials");

        ModelCreateArg shotgunModelArg{ressourceManager.get<Shader>("LightAndTexture"),
                                        vecMaterialsShotgun,
                                        ressourceManager.get<Mesh>("ShotgunMesh"),
                                        "LightAndTexture",
                                        "ShotgunMaterials",
                                        "ShotgunMesh"};

        GameObject& shotgunGO = _scene->add<GameObject>(player1GO, shotgunGameObject);
        shotgunGO.addComponent<Model>(shotgunModelArg);
        Firearm& shotgunComponent = shotgunGO.addComponent<Shotgun>(1.f, 1000.f, 10, 1.f, 2, 0.2f, 0.5,ressourceManager.get<Sound>("Shotgun"));
        playerControllerPlayer1.addFirearm(&shotgunComponent);
    }
    //SubMachinegun
    {
        GameObjectCreateArg subMachineGunGameObject {"SubMachineGun",
                                                    {{-0.5f, -1.5f, 3.2f},
                                                    {0.f, M_PI, 0.f},
                                                    {0.3f, 0.3f, 0.3f}}};

        std::vector<Material>* vecMaterialsSubMachineGun = ressourceManager.get<std::vector<Material>>("SubMachineGunMaterials");

        ModelCreateArg subMachineGunModelArg{ressourceManager.get<Shader>("LightAndTexture"),
                                        vecMaterialsSubMachineGun,
                                        ressourceManager.get<Mesh>("SubMachineGunMesh"),
                                        "LightAndTexture",
                                        "SubMachineGunMaterials",
                                        "SubMachineGunMesh"};

        GameObject& subMachineGunGO = _scene->add<GameObject>(player1GO, subMachineGunGameObject);
        subMachineGunGO.addComponent<Model>(subMachineGunModelArg);
        Firearm& subMachineGunComponent = subMachineGunGO.addComponent<SubMachineGun>(1.f, 1000.f, 1, 1.f, 30, 0.1f,ressourceManager.get<Sound>("Machinegun"));
        playerControllerPlayer1.addFirearm(&subMachineGunComponent);
    }

    //load billboards
    std::vector<Material> *vecMaterialsPseudo = ressourceManager.get<std::vector<Material>>("PseudoMaterial");
    Size textureSize = (*vecMaterialsPseudo)[0].getPDiffuseTexture()->getSize();

    GameObjectCreateArg pseudoGameObject{"Pseudo",
                                         {{0.0f, 5.0f, 0.0f},
                                          {0.f, 0.f, 0.f},
                                          {-textureSize.width / 10.f, 1.f, textureSize.height / 10.f}}};

    ModelCreateArg planeArg{ressourceManager.get<Shader>("TextureOnly"),
                            vecMaterialsPseudo,
                            ressourceManager.get<Mesh>("Plane"),
                            "TextureOnly",
                            "PseudoMaterial",
                            "Plane", true, false};

    _scene->add<GameObject>(player1GO, pseudoGameObject).addComponent<BillBoard>(planeArg);
}

void Demo::loadGround(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg groundArgGameObject{"Ground",
                                            {{0.f, 0.f, 0.f},
                                             {0.f, 0.f, 0.f},
                                             {3000.f, 1.f, 3000.f}}};

    ModelCreateArg groundArg{ressourceManager.get<Shader>("LightAndTexture"),
                             ressourceManager.get<std::vector<Material>>("Ground"),
                             ressourceManager.get<Mesh>("GroundMesh"),
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

void Demo::loadFog           (t_RessourcesManager& ressourceManager, unsigned int number)
{
   GameObjectCreateArg fogGameObjectArg  {"FogContener"};

    GameObject& fogContener = _scene->add<GameObject>(_scene->getWorld(), fogGameObjectArg);

    std::vector<Material>* vecMaterials = ressourceManager.get<std::vector<Material>>("FogMaterials");
    
    ModelCreateArg fogModelArg{ressourceManager.get<Shader>("BillBoardFogShader"),
                            vecMaterials,
                            ressourceManager.get<Mesh>("BillBoardFogMesh"),
                            "BillBoardFogShader",
                            "FogMaterials",
                            "BillBoardFogMesh",
                            true, false, false};
    
    /*Create tree with random size, position and rotation and add it on tre contener*/
    float rotAngleRadStep = M_PI * 2.f / number;
    for (size_t i = 0; i < number; i++)
    {
        fogGameObjectArg.name = "Fog" + std::to_string(i);
        fogGameObjectArg.transformArg.position.x = cos(i * rotAngleRadStep) * (i % 2 == 0 ? 1000.f : 800.f);
        fogGameObjectArg.transformArg.position.z = sin(i * rotAngleRadStep) * (i % 2 == 0 ? 1000.f : 800.f);
        fogGameObjectArg.transformArg.rotation.y = i * -rotAngleRadStep + M_PI_2;
        fogGameObjectArg.transformArg.scale = {500.f, 200.f, 1.f};
        fogGameObjectArg.transformArg.position.y = fogGameObjectArg.transformArg.scale.y / 10.f * 4.f;

        _scene->add<GameObject>(fogContener, fogGameObjectArg).addComponent<Model>(fogModelArg);
    }
}

void Demo::loadLootMachin              (t_RessourcesManager& ressourceManager)
{
    GameObject& lotsContener = _scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"LotsContener"});


    GameObjectCreateArg lootMachinArgGameObject{"LootMachine",
                                            {{-30.f, 5.f, -80.f},
                                             {0.f, 0.f, 0.f},
                                             {1.f, 1.f, 1.f}}};

    GameObject& lootMachin = _scene->add<GameObject>(_scene->getWorld(), lootMachinArgGameObject);
    lootMachin.addComponent<LootMachine>();

    float wrapHeight = 10.f;
    float wrapWidth = 4.f;
    float wrapDepth = 2.5f;
    float wrapThickness = 0.6f;

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
                                             
    ModelCreateArg cylinderArg{ressourceManager.get<Shader>("ColorWithLight"),
                             ressourceManager.get<std::vector<Material>>("BlackMaterial"),
                             ressourceManager.get<Mesh>("Cylinder"),
                             "ColorWithLight",
                             "BlackMaterial",
                             "Cylinder", 
                            true, false};

    _scene->add<GameObject>(lever, cylinderArgGameObject).addComponent<Model>(cylinderArg);

     GameObjectCreateArg sphereArgGameObject{"Sphere",
                                            {{0.f, cylinderArgGameObject.transformArg.scale.z, 0.f},
                                            {0, 0.f, 0.f},
                                            {1.f, 1.f, 1.f}}};
                                             
    ModelCreateArg sphereArg{ressourceManager.get<Shader>("ColorWithLight"),
                             ressourceManager.get<std::vector<Material>>("RedMaterial"),
                             ressourceManager.get<Mesh>("Sphere"),
                             "ColorWithLight",
                             "RedMaterial",
                             "Sphere"};

    _scene->add<GameObject>(lever, sphereArgGameObject).addComponent<Model>(sphereArg);

    GameObjectCreateArg cylinderBaseArgGameObject{"CylinderBase",
                                            {{-0.25f, 0.f, 0.f},
                                            {0.f, M_PI_2, 0.f},
                                            {0.7f, 0.7f, 1.5f}}};
                                             
    ModelCreateArg cylinderBaseArg{ressourceManager.get<Shader>("ColorWithLight"),
                             ressourceManager.get<std::vector<Material>>("BlackMaterial"),
                             ressourceManager.get<Mesh>("Cylinder"),
                             "ColorWithLight",
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
                                            {{wrapWidth / 4.f - 0.1f, (wrapHeight / 8.f * 6) - wrapHeight / 2.f, 0.f},
                                            {0.f, 0.f, M_PI_4},
                                            {wrapWidth / 2.f, wrapThickness / 2.f, wrapDepth}}};

    GameObjectCreateArg secondInclinedPlatformArgGameObject{"SecondInclinedPlatform",
                                            {{-wrapWidth / 4.f + 0.1f, (wrapHeight / 8.f * 5) - wrapHeight / 2.f, 0.f},
                                            {0.f, 0.f, -M_PI_4},
                                            {wrapWidth / 2.f, wrapThickness / 2.f, wrapDepth}}};

    GameObjectCreateArg thirdInclinedPlatformArgGameObject{"ThirdInclinedPlatform",
                                            {{wrapWidth / 4.f - 0.1f, (wrapHeight / 8.f * 4) - wrapHeight / 2.f, 0.f},
                                            {0.f, 0.f, M_PI_4},
                                            {wrapWidth / 2.f, wrapThickness / 2.f, wrapDepth}}};
                        
    GameObjectCreateArg forthInclinedPlatformArgGameObject{"ForthInclinedPlatform",
                                            {{-wrapWidth / 4.f + 0.1f, (wrapHeight / 8.f * 3) - wrapHeight / 2.f, 0.f},
                                            {0.f, 0.f, -M_PI_4},
                                            {wrapWidth / 2.f, wrapThickness / 2.f, wrapDepth}}};
                            
    GameObjectCreateArg distributorInclinedPlatformArgGameObject{"DistributorInclinedPlatform",
                                            {{0.f, (wrapHeight / 8.f * 1) - wrapHeight / 2.f, 0.f},
                                            {M_PI_4, 0.f, 0.f},
                                            {wrapWidth, wrapThickness / 2.f, wrapDepth}}};
                                             
    ModelCreateArg greenPlatformArg{ressourceManager.get<Shader>("ColorWithLight"),
                             ressourceManager.get<std::vector<Material>>("GreenMaterial"),
                             ressourceManager.get<Mesh>("Cube"),
                            "ColorWithLight",
                            "GreenMaterial",
                            "Cube"};

    ModelCreateArg pinkPlatformArg{ressourceManager.get<Shader>("ColorWithLight"),
                             ressourceManager.get<std::vector<Material>>("PinkMaterial"),
                             ressourceManager.get<Mesh>("Cube"),
                            "ColorWithLight",
                            "PinkMaterial",
                            "Cube"};

    ModelCreateArg redPlatformArg{ressourceManager.get<Shader>("ColorWithLight"),
                             ressourceManager.get<std::vector<Material>>("RedMaterial"),
                             ressourceManager.get<Mesh>("Cube"),
                            "ColorWithLight",
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
                                            
    ModelCreateArg blackPlatformArg{ressourceManager.get<Shader>("ColorWithLight"),
                             ressourceManager.get<std::vector<Material>>("BlackMaterial"),
                             ressourceManager.get<Mesh>("Cube"),
                            "ColorWithLight",
                            "BlackMaterial",
                            "Cube"};

    ModelCreateArg glassPlatformArg{ressourceManager.get<Shader>("LightAndTexture"),
                             ressourceManager.get<std::vector<Material>>("GlassMaterial"),
                             ressourceManager.get<Mesh>("Cube"),
                            "LightAndTexture",
                            "GlassMaterial",
                            "Cube"};
    glassPlatformArg.isOpaque = false;

    GameObject& wrap1GO = _scene->add<GameObject>(wrap, rightFaceArgGameObject);
    wrap1GO.addComponent<Model>(blackPlatformArg);
    wrap1GO.addComponent<OrientedBoxCollider>();

    GameObject& wrap2GO = _scene->add<GameObject>(wrap, leftFaceArgGameObject);
    wrap2GO.addComponent<Model>(blackPlatformArg);
    wrap2GO.addComponent<OrientedBoxCollider>();

    GameObject& wrap3GO = _scene->add<GameObject>(wrap, backwardFaceArgGameObject);
    wrap3GO.addComponent<Model>(blackPlatformArg);
    wrap3GO.addComponent<OrientedBoxCollider>();

    GameObject& wrap4GO = _scene->add<GameObject>(wrap, topFaceArgGameObject);
    wrap4GO.addComponent<Model>(blackPlatformArg);

    GameObject& wrap5GO = _scene->add<GameObject>(wrap, forwardTopFaceArgGameObject);
    wrap5GO.addComponent<Model>(blackPlatformArg);

    GameObject& wrap6GO = _scene->add<GameObject>(wrap, forwardGlassFaceArgGameObject);
    wrap6GO.addComponent<Model>(glassPlatformArg);

    /*Create lots prefabs*/
    {
        GameObjectCreateArg lot1GameObjectArg{"Lot1"};
        lot1GameObjectArg.transformArg.scale /= 2.f;

        GameObject& lot1GO = _scene->add<GameObject>(_scene->getWorld(), lot1GameObjectArg);

        ModelCreateArg modelBlueCrateArg{ressourceManager.get<Shader>("LightAndTexture"),
                            ressourceManager.get<std::vector<Material>>("YellowCrateMaterial"),
                            ressourceManager.get<Mesh>("Cube"),
                            "LightAndTexture",
                            "YellowCrateMaterial",
                            "Cube"};

        lot1GO.addComponent<Model>(modelBlueCrateArg);
        lot1GO.addComponent<PhysicalObject>().setMass(1);
        SphereCollider& colliderComp = lot1GO.addComponent<SphereCollider>();
        colliderComp.setBounciness(0.4f);
        colliderComp.setFriction(0.9f);
        lot1GO.addComponent<BombeLoot>();

        Save::createPrefab(lot1GO, "Lot1");
        lot1GO.destroyImmediate();
    }

    {
        GameObjectCreateArg lot2GameObjectArg{"Lot2"};
        lot2GameObjectArg.transformArg.scale /= 2.f;

        GameObject& lot2GO = _scene->add<GameObject>(_scene->getWorld(), lot2GameObjectArg);

        ModelCreateArg modelCrateArg{ressourceManager.get<Shader>("LightAndTexture"),
                            ressourceManager.get<std::vector<Material>>("RedCrateMaterial"),
                            ressourceManager.get<Mesh>("Cube"),
                            "LightAndTexture",
                            "RedCrateMaterial",
                            "Cube"};

        lot2GO.addComponent<Model>(modelCrateArg);
        lot2GO.addComponent<PhysicalObject>().setMass(1);
        SphereCollider& colliderComp = lot2GO.addComponent<SphereCollider>();
        colliderComp.setBounciness(0.1f);
        colliderComp.setFriction(0.95f);
        lot2GO.addComponent<LifeLoot>();

        Save::createPrefab(lot2GO, "Lot2");
        lot2GO.destroyImmediate();
    }

    /*Create spawner*/

    GameObjectCreateArg spawnerGOArg;
    {
        spawnerGOArg.name = "Spawner";
        spawnerGOArg.transformArg.position = {wrapWidth / 4.f, wrapHeight / 2.f - 1.f, 0.f};

        GameObject& spawnerGO = _scene->add<GameObject>(lootMachin, spawnerGOArg);
        spawnerGO.addComponent<CircularEntitiesSpawner>(nullptr,0.1f, 0.5f, 0.f).setContenor(&lotsContener);

        //Save::createPrefab(spawnerGO, spawnerGOArg.name);
        //spawnerGO.destroyImmediate();
    }
}

void createUpgradeStation(std::unique_ptr<Scene>& scene, t_RessourcesManager& ressourceManager, GameObject& parent, std::string mat)
{
    {// Back
        GameObjectCreateArg plankGOArg{"upgradePlankBack",
                                    {{0.f, 0.f, 0.f},
                                    {0.f, 0.f, 0.f},
                                    {5.f, 5.f, 0.5f}}};

        GameObject& plank = scene->add<GameObject>(parent, plankGOArg);

        ModelCreateArg plankModelArg{ressourceManager.get<Shader>("LightAndTexture"),
                                ressourceManager.get<std::vector<Material>>("BlackMaterial"),
                                ressourceManager.get<Mesh>("Cube"),
                                "LightAndTexture",
                                "BlackMaterial",
                                "Cube"};

        plank.addComponent<Model>(plankModelArg);
        plank.addComponent<OrientedBoxCollider>();
    }
    { // Left
        GameObjectCreateArg plankGOArg{"upgradePlankLeft",
                                    {{-2.5f, 0.f, 1.f},
                                    {0.f, M_PI, 0.f},
                                    {0.5f, 5.f, 2.f}}};

        GameObject& plank = scene->add<GameObject>(parent, plankGOArg);

        ModelCreateArg plankModelArg{ressourceManager.get<Shader>("LightAndTexture"),
                                ressourceManager.get<std::vector<Material>>("BlackMaterial"),
                                ressourceManager.get<Mesh>("Cube"),
                                "LightAndTexture",
                                "BlackMaterial",
                                "Cube"};

        plank.addComponent<Model>(plankModelArg);
        plank.addComponent<OrientedBoxCollider>();
    }
    { // right
        GameObjectCreateArg plankGOArg{"upgradePlankRight",
                                    {{2.5f, 0.f, 1.f},
                                    {0.f, M_PI, 0.f},
                                    {0.5f, 5.f, 2.f}}};

        GameObject& plank = scene->add<GameObject>(parent, plankGOArg);

        ModelCreateArg plankModelArg{ressourceManager.get<Shader>("LightAndTexture"),
                                ressourceManager.get<std::vector<Material>>("BlackMaterial"),
                                ressourceManager.get<Mesh>("Cube"),
                                "LightAndTexture",
                                "BlackMaterial",
                                "Cube"};

        plank.addComponent<Model>(plankModelArg);
        plank.addComponent<OrientedBoxCollider>();
    }
    {// Deck
        GameObjectCreateArg plankGOArg{"upgradePlanDeck",
                                    {{0.f, 1.3f, 0.5f},
                                    {0.f, 0.f, 0.f},
                                    {5.f, 0.5f, 1.3f}}};

        GameObject& plank = scene->add<GameObject>(parent, plankGOArg);

        ModelCreateArg plankModelArg{ressourceManager.get<Shader>("LightAndTexture"),
                                ressourceManager.get<std::vector<Material>>("BlackMaterial"),
                                ressourceManager.get<Mesh>("Cube"),
                                "LightAndTexture",
                                "BlackMaterial",
                                "Cube"};

        plank.addComponent<Model>(plankModelArg);
        plank.addComponent<OrientedBoxCollider>();
    }
    {// Button
        GameObjectCreateArg sphereGOarg{"upgradeButton",
                                    {{0.f, 1.5f, 0.5f},
                                    {0.f, 0.f, 0.f},
                                    {0.5f, 0.5f, 0.5f}}};

        GameObject& sphereGO = scene->add<GameObject>(parent, sphereGOarg);

        ModelCreateArg sphereModelArg{ressourceManager.get<Shader>("LightAndTexture"),
                                ressourceManager.get<std::vector<Material>>("YellowMaterial"),
                                ressourceManager.get<Mesh>("Sphere"),
                                "LightAndTexture",
                                "YellowMaterial",
                                "Sphere"};

        sphereGO.addComponent<Model>(sphereModelArg);
    }
    {// Front
        GameObjectCreateArg plankGOArg{"upgradePlankFront",
                                    {{0.f, -0.5f, 1.4f},
                                    {degres_to_rad(-10.f), 0.f, 0.f},
                                    {5.f, 4.f, 0.5f}}};

        GameObject& plank = scene->add<GameObject>(parent, plankGOArg);

        ModelCreateArg plankModelArg{ressourceManager.get<Shader>("LightAndTexture"),
                                ressourceManager.get<std::vector<Material>>(mat),
                                ressourceManager.get<Mesh>("Cube"),
                                "LightAndTexture",
                                mat,
                                "Cube"};

        plank.addComponent<Model>(plankModelArg);
        plank.addComponent<OrientedBoxCollider>();
    }
    {
        GameObjectCreateArg lightGOarg{"Light",
                                    {{0.f, 0.f, 0.4f},
                                    {0.f, 0.f, 0.f},
                                    {1.f, 1.f, 1.f}}};

        GameObject& lightGO = scene->add<GameObject>(parent, lightGOarg);

        PointLightCreateArg lightArg{ {0.5f, 0.2f, 0.2f, 0.2f},
                                    {0.5f, 0.2f, 0.2f, 0.5f},
                                    {0.5f, 0.2f, 0.2f, 0.7f},
                                    0.f, 0.2f, 0.f, true};
        
        lightGO.addComponent<PointLight>(lightArg);
    }
}

void Demo::loadUpgradeStation         (t_RessourcesManager& ressourceManager)
{
    {
        GameObjectCreateArg upgradeStationGOArg{"ReloadTimeUpgradeStation",
                                                {{30.f, 2.5f, -50.f},
                                                {0.f, -M_PI_4, 0.f},
                                                {1.f, 1.f, 1.f}}};

        GameObject& upgradeStationGO = _scene->add<GameObject>(_scene->getWorld(), upgradeStationGOArg);
        upgradeStationGO.addComponent<ReloadTimeUpgradeStation>(1000);

        createUpgradeStation(_scene, ressourceManager, upgradeStationGO, "ReloadMaterial");
    }
    {
        GameObjectCreateArg upgradeStationGOArg{"AutoUpgradeStation",
                                                {{-75.f, 2.5f, -20.f},
                                                {0.f, M_PI_4, 0.f},
                                                {1.f, 1.f, 1.f}}};

        GameObject& upgradeStationGO = _scene->add<GameObject>(_scene->getWorld(), upgradeStationGOArg);
        upgradeStationGO.addComponent<AutoUpgradeStation>(2000);

        createUpgradeStation(_scene, ressourceManager, upgradeStationGO, "AutoMaterial");
    }
    {
        GameObjectCreateArg upgradeStationGOArg{"DamageUpgradeStation",
                                                {{75.f, 2.5f, -40.f},
                                                {0.f, -M_PI_4, 0.f},
                                                {1.f, 1.f, 1.f}}};

        GameObject& upgradeStationGO = _scene->add<GameObject>(_scene->getWorld(), upgradeStationGOArg);
        upgradeStationGO.addComponent<DamageUpgradeStation>(1500);

        createUpgradeStation(_scene, ressourceManager, upgradeStationGO, "DamageMaterial");
    }
    {
        GameObjectCreateArg upgradeStationGOArg{"FireRateUpgradeStation",
                                                {{65.f, 2.5f, 40.f},
                                                {0.f, -M_PI_4 - M_PI_2, 0.f},
                                                {1.f, 1.f, 1.f}}};

        GameObject& upgradeStationGO = _scene->add<GameObject>(_scene->getWorld(), upgradeStationGOArg);
        upgradeStationGO.addComponent<FireRateUpgradeStation>(1000);

        createUpgradeStation(_scene, ressourceManager, upgradeStationGO, "FireRateMaterial");
    }
    {
        GameObjectCreateArg upgradeStationGOArg{"MunitionCapacityUpgradeStation",
                                                {{-55.f, 2.5f, 40.f},
                                                {0.f, M_PI_2, 0.f},
                                                {1.f, 1.f, 1.f}}};

        GameObject& upgradeStationGO = _scene->add<GameObject>(_scene->getWorld(), upgradeStationGOArg);
        upgradeStationGO.addComponent<MunitionCapacityUpgradeStation>(800);

        createUpgradeStation(_scene, ressourceManager, upgradeStationGO, "MunitionMaterial");
    }
}         

void Demo::loadCamera()
{
    CameraPerspectiveCreateArg camArg{{0.f, 0.f, 30.f}, {0.f, 0.f, 0.f}, _gameEngine.getWinSize().width / static_cast<float>(_gameEngine.getWinSize().height), 0.1f, 10000.0f, 45.0f, "MainCamera"};
    mainCamera = &_scene->add<Camera>(_scene->getWorld(), camArg);
    dynamic_cast<Camera *>(mainCamera)->use();
}

void Demo::loadLights()
{
    GameObjectCreateArg lightSphereGameObjectArg{"Sun",
                                                 {{0.f, 0.f, 0.f},
                                                  {0.f, 0.f, 0.f},
                                                  {1.f, 1.f, 1.f}}};

    DirectionnalLightCreateArg lightArg2{{0.f, 1.f, -1.f},
                                         {1.f, 1.f, 1.f, 0.1f},
                                         {1.f, 1.f, 1.f, 0.7f},
                                         {1.f, 1.f, 1.f, 1.f},
                                         true};

    GameObject &pl = _scene->add<GameObject>(_scene->getWorld(), lightSphereGameObjectArg);
    pl.addComponent<DirectionnalLight>(lightArg2);
}

void Demo::loadGUI(t_RessourcesManager &ressourceManager)
{
    int halfWidth = _gameEngine.getWinSize().width / 2.0f;
    int halfHeight = _gameEngine.getWinSize().height / 2.0f;
    float crosshairSize = _gameEngine.getWinSize().height / 15;
    float halfcrosshairSize = crosshairSize * 0.5;

    if (ressourceManager.get<ReferencedTitle>("WaveIndicatorUI"))
        ressourceManager.remove<ReferencedTitle>("WaveIndicatorUI");
    ressourceManager.add<ReferencedTitle>("WaveIndicatorUI", ressourceManager.get<Font>("font2"), ressourceManager.get<Shader>("ButtonShader"),
                                _gameEngine.getWinSize().width / 2.0f  - _gameEngine.getWinSize().width / 2.0f / 10.f, _gameEngine.getWinSize().height / 2.0f - _gameEngine.getWinSize().height / 2.0f / 10.f * 9.f,
                                150.0f, 60.0f, SDL_Color{200, 30, 30, 255}, (int*)_scene->getGameObject("waveManager").getComponent<WaveManager>()->getPCurrentWave(),"Wave ", E_GAME_STATE::RUNNING);
    
    Texture* t_hitmarker = ressourceManager.get<Texture>("hitmarker");

    if (ressourceManager.get<Image>("hitmarkerImage"))
        ressourceManager.remove<Image>("hitmarkerImage");
    Image* hitmarker = &ressourceManager.add<Image>("hitmarkerImage",
                                                    t_hitmarker->getID(),
                                                    ressourceManager.get<Shader>("ImageShader"),
                                                    halfWidth - halfcrosshairSize,
                                                    halfHeight - halfcrosshairSize,
                                                    crosshairSize,
                                                    crosshairSize, 
                                                    E_GAME_STATE::RUNNING);

    hitmarker->setName("HitMarker");
    hitmarker->isActive = false;

    Texture* t_crosshair = ressourceManager.get<Texture>("crosshair");

    if (ressourceManager.get<Image>("CrosshairImage"))
        ressourceManager.remove<Image>("CrosshairImage");
    ressourceManager.add<Image>("CrosshairImage",
                                t_crosshair->getID(),
                                ressourceManager.get<Shader>("ImageShader"),
                                halfWidth - halfcrosshairSize,
                                halfHeight - halfcrosshairSize,
                                crosshairSize,
                                crosshairSize,
                                E_GAME_STATE::RUNNING);

    Texture* t_health = ressourceManager.get<Texture>("Health");
    if (ressourceManager.get<Image>("HealthImage"))
        ressourceManager.remove<Image>("HealthImage");
    ressourceManager.add<Image>("HealthImage",
                                t_health->getID(),
                                ressourceManager.get<Shader>("ImageShader"),
                                0,
                                halfHeight*2 - t_health->getSize().height,
                                t_health->getSize().width,
                                t_health->getSize().height,
                                E_GAME_STATE::RUNNING);

    Texture* t_bullet = ressourceManager.get<Texture>("bullet");
    if (ressourceManager.get<Image>("BulletImage"))
        ressourceManager.remove<Image>("BulletImage");
    ressourceManager.add<Image>("BulletImage",
                                t_bullet->getID(),
                                ressourceManager.get<Shader>("ImageShader"),
                                halfWidth*2 - t_bullet->getSize().width - t_bullet->getSize().width/2,
                                halfHeight*2 - t_bullet->getSize().height - t_bullet->getSize().height/15,
                                t_bullet->getSize().width,
                                t_bullet->getSize().height,
                                E_GAME_STATE::RUNNING);

    if (ressourceManager.get<ReferencedTitle>("lifeIndicator"))
        ressourceManager.remove<ReferencedTitle>("lifeIndicator");
    ressourceManager.add<ReferencedTitle>("lifeIndicator", ressourceManager.get<Font>("font1"), ressourceManager.get<Shader>("ButtonShader"),
                                halfWidth/10.f, halfHeight*2 - halfHeight/10.f,
                                halfWidth, halfWidth/10.f, SDL_Color{240, 240, 240, 255},_scene->getGameObject("world/Players/Player1").getComponent<PlayerController>()->getLife(),"", E_GAME_STATE::RUNNING);
    
    if (ressourceManager.get<ReferencedTitle>("maxlifeIndicator"))
        ressourceManager.remove<ReferencedTitle>("maxlifeIndicator");
    ressourceManager.add<ReferencedTitle>("maxlifeIndicator", ressourceManager.get<Font>("font1"), ressourceManager.get<Shader>("ButtonShader"),
                                halfWidth/5.f, halfHeight*2 - halfHeight/10.f,
                                halfWidth, halfWidth/10.f, SDL_Color{240, 240, 240, 255},_scene->getGameObject("world/Players/Player1").getComponent<PlayerController>()->getMaxLife(),"/ ", E_GAME_STATE::RUNNING);                        

    if (ressourceManager.get<ReferencedTitle>("moneyIndicator"))
        ressourceManager.remove<ReferencedTitle>("moneyIndicator");
    ressourceManager.add<ReferencedTitle>("moneyIndicator", ressourceManager.get<Font>("font1"), ressourceManager.get<Shader>("ButtonShader"),
                                halfWidth/10.f, halfHeight/10.f,
                                halfWidth, halfWidth/10.f, SDL_Color{120, 240, 120, 255},_scene->getGameObject("world/Players/Player1").getComponent<PlayerController>()->getMoney(),"$", E_GAME_STATE::RUNNING);                        
    
    if (ressourceManager.get<Title>("bulletIndicator"))
        ressourceManager.remove<Title>("bulletIndicator");
    Title* bulletIndicator = &ressourceManager.add<Title>("bulletIndicator", ressourceManager.get<Font>("font1"), ressourceManager.get<Shader>("ButtonShader"),
                                halfWidth*1.75, halfHeight*2 - halfHeight/10.f,
                                halfWidth, halfWidth/10.f, SDL_Color{240, 240, 240, 255}," Bullets", E_GAME_STATE::RUNNING);
    
    _scene->getGameObject("world/Players/Player1").getComponent<PlayerController>()->setBulletReference(bulletIndicator);

}

void Demo::loadUI(t_RessourcesManager &ressourceManager)
{
    FontCreateArg fontarg{"./ressources/font/opensans.ttf", 40};
    Font *pfont = &ressourceManager.add<Font>("font1", fontarg);
    FontCreateArg fontarg2{"./ressources/font/opensans.ttf", 25};
    Font *pfont2 = &ressourceManager.add<Font>("font2", fontarg2);
    Shader *buttonShader = &ressourceManager.add<Shader>("ButtonShader", "./ressources/shader/text.vs", "./ressources/shader/texture.fs");

    int halfWidth = _gameEngine.getWinSize().width / 2.0f;
    int halfHeight = _gameEngine.getWinSize().height / 2.0f;

#pragma region Start

    ressourceManager.add<Button>("MenuStartButton", pfont, buttonShader,
                                 halfWidth - 90, halfHeight - 200,
                                 200.0f, 60.0f, SDL_Color{170, 80, 80, 0}, "New Game",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {

        _scene = std::make_unique<Scene>();
        
        loadCamera();
        loadEntity(_gameEngine.ressourceManager_);
        loadLights();
        loadBulletHoleContenor (50);
        loadEnemies(_gameEngine.ressourceManager_);
        loadUpgradeStation(_gameEngine.ressourceManager_);
        loadGUI(_gameEngine.ressourceManager_);
    
        ScriptSystem::start();
        _gameEngine.gameState = E_GAME_STATE::RUNNING;
        usingMouse = false;
    };

    ressourceManager.add<Button>("MenuLoadButton", pfont, buttonShader,
                                 halfWidth - 95, halfHeight - 100,
                                 220.0f, 60.0f, SDL_Color{170, 170, 80, 0}, "Load Game",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::STARTSAVE;
    };

    ressourceManager.add<Button>("MenuOptionButton", pfont, buttonShader,
                                 halfWidth - 65, halfHeight,
                                 150.0f, 60.0f, SDL_Color{80, 170, 170, 0}, "Options",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::OPTION;
    };

    ressourceManager.add<Button>("MenuQuitButton", pfont, buttonShader,
                                 halfWidth - 35, halfHeight + 100,
                                 150.0f, 60.0f, SDL_Color{80, 80, 170, 0}, "Quit",
                                 E_GAME_STATE::STARTING)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::EXIT;
    };

#pragma endregion

#pragma region Pause

    ressourceManager.add<Button>("PausePlayButton", pfont, buttonShader,
                                 halfWidth - 35, halfHeight - 100,
                                 150.0f, 60.0f, SDL_Color{170, 80, 170, 0}, "Play",
                                 E_GAME_STATE::PAUSE)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::RUNNING;
        usingMouse = false;
    };

    ressourceManager.add<Button>("PauseMenuButton", pfont, buttonShader,
                                 halfWidth - 45, halfHeight,
                                 150.0f, 60.0f, SDL_Color{80, 170, 80, 0}, "Menu",
                                 E_GAME_STATE::PAUSE)
        .function = [&]() {

        Light::resetLight();
        _scene.reset();
        _gameEngine.gameState = E_GAME_STATE::STARTING;
        usingMouse = true;
    };

#pragma endregion

#pragma region Option

    int decal = 250;

    ressourceManager.add<Title>("OptionForwardTitle", pfont, buttonShader,
                                halfWidth - decal - 155, halfHeight - 400,
                                175.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Forward :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionForwardButton", pfont, buttonShader,
                                 halfWidth - decal + 50, halfHeight - 400,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.up)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.up = key;
            ressourceManager.get<Button>("OptionForwardButton")->value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionForwardButton")->updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionBackwardTitle", pfont, buttonShader,
                                halfWidth - decal - 185, halfHeight - 300,
                                200.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Backward :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionBackwardButton", pfont, buttonShader,
                                 halfWidth - decal + 50, halfHeight - 300,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.down)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.down = key;
            ressourceManager.get<Button>("OptionBackwardButton")->value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionBackwardButton")->updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionLeftTitle", pfont, buttonShader,
                                halfWidth - decal - 75, halfHeight - 200,
                                150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Left :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionLeftButton", pfont, buttonShader,
                                 halfWidth - decal + 50, halfHeight - 200,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.left)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.left = key;
            ressourceManager.get<Button>("OptionLeftButton")->value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionLeftButton")->updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionRightTitle", pfont, buttonShader,
                                halfWidth - decal - 100, halfHeight - 100,
                                150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Right :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionRightButton", pfont, buttonShader,
                                 halfWidth - decal + 50, halfHeight - 100,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.right)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.right = key;
            ressourceManager.get<Button>("OptionRightButton")->value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionRightButton")->updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionJumpTitle", pfont, buttonShader,
                                halfWidth - decal - 105, halfHeight,
                                150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Jump :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionJumpButton", pfont, buttonShader,
                                 halfWidth - decal + 50, halfHeight,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.jump)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.jump = key;
            ressourceManager.get<Button>("OptionJumpButton")->value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionJumpButton")->updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Button>("OptionSwitchButton", pfont, buttonShader,
                                 halfWidth - decal - 45, halfHeight + 100,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "WASD",
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        Button *switchButton = ressourceManager.get<Button>("OptionSwitchButton");
        Button *upButton = ressourceManager.get<Button>("OptionForwardButton");
        Button *downButton = ressourceManager.get<Button>("OptionBackwardButton");
        Button *rightButton = ressourceManager.get<Button>("OptionRightButton");
        Button *leftButton = ressourceManager.get<Button>("OptionLeftButton");
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

    ressourceManager.add<Title>("OptionFlashLightTitle", pfont, buttonShader,
                                halfWidth + decal - 200, halfHeight - 400,
                                400.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Flash Light :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionFlashLightButton", pfont, buttonShader,
                                 halfWidth + decal + 50, halfHeight - 400,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.flashLight)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.flashLight = key;
            ressourceManager.get<Button>("OptionFlashLightButton")->value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionFlashLightButton")->updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionReloadTitle", pfont, buttonShader,
                                halfWidth + decal - 130, halfHeight - 300,
                                400.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Reload :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionReloadButton", pfont, buttonShader,
                                 halfWidth + decal + 50, halfHeight - 300,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.reload)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.reload = key;
            ressourceManager.get<Button>("OptionReloadButton")->value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionReloadButton")->updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Title>("OptionUseTitle", pfont, buttonShader,
                                halfWidth + decal - 72, halfHeight - 200,
                                400.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Use :",
                                E_GAME_STATE::OPTION);

    ressourceManager.add<Button>("OptionUseButton", pfont, buttonShader,
                                 halfWidth + decal + 50, halfHeight - 200,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, SDL_GetKeyName(SDL_GetKeyFromScancode(Input::keyboard.use)),
                                 E_GAME_STATE::OPTION)
        .function = [&]() {
        SDL_Scancode key = Input::waitForKey();
        if (key != SDL_SCANCODE_UNKNOWN && key != SDL_SCANCODE_ESCAPE)
        {
            Input::keyboard.use = key;
            ressourceManager.get<Button>("OptionUseButton")->value = SDL_GetKeyName(SDL_GetKeyFromScancode(key));
            ressourceManager.get<Button>("OptionUseButton")->updateTexture();
            Input::resetKeyDown();
        }
    };

    ressourceManager.add<Button>("Return", pfont, buttonShader,
                                 halfWidth - 50, halfHeight + 300,
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
                                     halfWidth + i, halfHeight + j,
                                     90.0f, 60.0f, SDL_Color{200, 200, 200, 0},
                                     shortSaveName, E_GAME_STATE::STARTSAVE)
            .function = [&]() {
            _gameEngine.gameState = E_GAME_STATE::RUNNING;
            usingMouse = false;
            Light::resetLight();
            _scene.reset();
            _scene = std::make_unique<Scene>();
            setupScene(*_scene, saves.c_str());
            loadGUI(_gameEngine.ressourceManager_);
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
                                 halfWidth - 50, halfHeight + 250,
                                 150.0f, 60.0f, SDL_Color{200, 200, 200, 0}, "Return",
                                 E_GAME_STATE::STARTSAVE)
        .function = [&]() {
        _gameEngine.gameState = E_GAME_STATE::STARTING;
    };

#pragma endregion
}

void Demo::loadEnemies(t_RessourcesManager &ressourceManager)
{
    GameObjectCreateArg NexusGameObjectArg{"Nexus"};

    GameObject& nexus = _scene->add<GameObject>(_scene->getWorld(), NexusGameObjectArg);

    ModelCreateArg modelNexusArg{ressourceManager.get<Shader>("LightAndTexture"),
                          ressourceManager.get<std::vector<Material>>("NexusMaterial"),
                          ressourceManager.get<Mesh>("NexusMesh"),
                          "LightAndTexture",
                          "NexusMaterial",
                          "NexusMesh"};
    modelNexusArg.isOpaque = false;
    nexus.addComponent<Model>(modelNexusArg);

    PointLightCreateArg lightArgNexusLight{{0.f, 0.2f, 1.f, 0.2f},
                                            {0.f, 0.2f, 1.f, 0.5f},
                                            {0.f, 0.2f, 1.f, 0.7f},
                                            0.f, 0.05f, 0.f, true};
    
    nexus.addComponent<PointLight>(lightArgNexusLight);

    nexus.setTranslation(Vec3{-10, 8, -10});
    nexus.setScale(Vec3{0.3f, 0.3f, 0.3f});
    
    nexus.addComponent<Nexus>();
    nexus.addComponent<LevitationMovement>(1.f, 1.f);

    GameObject* checkpoint1 = &_scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg {"checkpoint1"});
    GameObject* checkpoint2 = &_scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg {"checkpoint2"});
    GameObject* checkpoint3 = &_scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg {"checkpoint3"});
    GameObject* checkpoint4 = &_scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg {"checkpoint4"});

    enemiesContener = &_scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"EnemiesContener"});

    {
        GameObjectCreateArg Ennemy1GameObjectArg{"EyesBall"};
        Ennemy1GameObjectArg.transformArg.scale = {5.f, 5.f, 5.f};

        ModelCreateArg modelArg{ressourceManager.get<Shader>("LightAndTexture"),
                            ressourceManager.get<std::vector<Material>>("EyesBallMaterial"),
                            ressourceManager.get<Mesh>("EyesBallMesh"),
                            "LightAndTexture",
                            "EyesBallMaterial",
                            "EyesBallMesh"};

        GameObject& enemyBase = _scene->add<GameObject>(_scene->getWorld(), Ennemy1GameObjectArg);
        enemyBase.setTag("Enemy");

        enemyBase.addComponent<Model>(modelArg);

        PhysicalObject& physicalObjectComp = enemyBase.addComponent<PhysicalObject>();
        physicalObjectComp.setMass(1);
        physicalObjectComp.setFreezeRotX(true);
        physicalObjectComp.setFreezeRotY(true);
        physicalObjectComp.setFreezeRotZ(true);

        SphereCollider& sphereColliderComp = enemyBase.addComponent<SphereCollider>();
        sphereColliderComp.setBounciness(0.f);
        sphereColliderComp.setFriction(0.97f);

        enemyBase.addComponent<EnnemyController>(  &Scene::getCurrentScene()->getGameObject("world/Players/Player1"), 
                                                &Scene::getCurrentScene()->getGameObject("world/Nexus"));

        Save::createPrefab(enemyBase, "enemy1");
        enemyBase.destroyImmediate();
    }

    {
        GameObjectCreateArg Ennemy1GameObjectArg{"Spider"};
        Ennemy1GameObjectArg.transformArg.scale = {5.f, 5.f, 5.f};

        ModelCreateArg modelArg{ressourceManager.get<Shader>("LightAndTexture"),
                            ressourceManager.get<std::vector<Material>>("SpiderMaterial"),
                            ressourceManager.get<Mesh>("SpiderMesh"),
                            "LightAndTexture",
                            "SpiderMaterial",
                            "SpiderMesh"};

        GameObject& enemyBase = _scene->add<GameObject>(_scene->getWorld(), Ennemy1GameObjectArg);
        enemyBase.setTag("Enemy");

        enemyBase.addComponent<Model>(modelArg);

        PhysicalObject& physicalObjectComp = enemyBase.addComponent<PhysicalObject>();
        physicalObjectComp.setMass(1);
        physicalObjectComp.setFreezeRotX(true);
        physicalObjectComp.setFreezeRotY(true);
        physicalObjectComp.setFreezeRotZ(true);

        SphereCollider& sphereColliderComp = enemyBase.addComponent<SphereCollider>();
        sphereColliderComp.setBounciness(0.f);
        sphereColliderComp.setFriction(0.97f);

        EnnemyController& ennemyControllerComp = enemyBase.addComponent<EnnemyController>(  &Scene::getCurrentScene()->getGameObject("world/Players/Player1"), 
                                                                                            &Scene::getCurrentScene()->getGameObject("world/Nexus"));

        ennemyControllerComp.setLife(4);
        ennemyControllerComp.setSpeed(20);
        ennemyControllerComp.setDamage(2);
        ennemyControllerComp.setValueOnHit(10);
        ennemyControllerComp.setValueOnDeath(100);

        Save::createPrefab(enemyBase, "enemy2");
        enemyBase.destroyImmediate();
    }

    {
        GameObjectCreateArg Ennemy1GameObjectArg{"PlantMonster"};
        Ennemy1GameObjectArg.transformArg.scale = {5.f, 5.f, 5.f};

        ModelCreateArg modelArg{ressourceManager.get<Shader>("LightAndTexture"),
                            ressourceManager.get<std::vector<Material>>("PlantMonsterMaterial"),
                            ressourceManager.get<Mesh>("PlantMonsterMesh"),
                            "LightAndTexture",
                            "PlantMonsterMaterial",
                            "PlantMonsterMesh"};

        GameObject& enemyBase = _scene->add<GameObject>(_scene->getWorld(), Ennemy1GameObjectArg);
        enemyBase.setTag("Enemy");

        enemyBase.addComponent<Model>(modelArg);

        PhysicalObject& physicalObjectComp = enemyBase.addComponent<PhysicalObject>();
        physicalObjectComp.setMass(1);
        physicalObjectComp.setFreezeRotX(true);
        physicalObjectComp.setFreezeRotY(true);
        physicalObjectComp.setFreezeRotZ(true);

        SphereCollider& sphereColliderComp = enemyBase.addComponent<SphereCollider>();
        sphereColliderComp.setBounciness(0.f);
        sphereColliderComp.setFriction(0.97f);

        EnnemyController& ennemyControllerComp = enemyBase.addComponent<EnnemyController>(  &Scene::getCurrentScene()->getGameObject("world/Players/Player1"), 
                                                                                            &Scene::getCurrentScene()->getGameObject("world/Nexus"));

        ennemyControllerComp.setLife(8);
        ennemyControllerComp.setSpeed(35);
        ennemyControllerComp.setDamage(1);
        ennemyControllerComp.setRadius(50);
        ennemyControllerComp.setValueOnHit(10);
        ennemyControllerComp.setValueOnDeath(100);

        Save::createPrefab(enemyBase, "enemy3");
        enemyBase.destroyImmediate();
    }

    GameObjectCreateArg spawnerGOArg;

    GameObjectCreateArg pointLightGameObjectArg{"PointLight", {{0.f, 2.f, 0.f}}};
    PointLightCreateArg lightArg{{1.f, 1.f, 1.f, 0.f},
                                {1.f, 0.f, 0.f, 0.7f},
                                {1.f, 1.f, 1.f, 0.3f},
                                0.f, 0.05f, 0.f, true};
    ModelCreateArg modelSpawnerArg{ressourceManager.get<Shader>("ColorWithLight"),
                            ressourceManager.get<std::vector<Material>>("BlueMaterial"),
                            ressourceManager.get<Mesh>("Sphere"),
                            "ColorWithLight",
                            "BlueMaterial",
                            "Sphere"};
    {
        spawnerGOArg.name = "Spawner1";
        spawnerGOArg.transformArg.position = {-120.f, 5.f, -175.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        checkpoint1->addComponent<Checkpoint>().addCheckpoint(Vec3{-72.f, 5.f, -145.f});
        checkpoint1->getComponent<Checkpoint>()->addCheckpoint(Vec3{-65.f, 5.f, -90.f});
        checkpoint1->getComponent<Checkpoint>()->addCheckpoint(Vec3{-36.f, 5.f, -10.f});
        checkpoint1->getComponent<Checkpoint>()->addCheckpoint(Vec3{-8.f, 5.f, 1.f});
        checkpoint1->getComponent<Checkpoint>()->addCheckpoint(Vec3{-10.f, 5.f, -10.f});
        spawnerGO.addComponent<CircularEntitiesSpawner>(checkpoint1->getComponent<Checkpoint>(), 2.f, 0.5f, 0.f).setContenor(enemiesContener);
        spawnerGO.addComponent<Model>(modelSpawnerArg);
        _scene->add<GameObject>(spawnerGO, pointLightGameObjectArg).addComponent<PointLight>(lightArg);
        

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroyImmediate();
    }

    {
        spawnerGOArg.name = "Spawner2";
        spawnerGOArg.transformArg.position = {-150.f, 5.f, 59.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        checkpoint2->addComponent<Checkpoint>().addCheckpoint(Vec3{-116.f, 5.f, 21.f});
        checkpoint2->getComponent<Checkpoint>()->addCheckpoint(Vec3{-95.f, 5.f, -6.f});
        checkpoint2->getComponent<Checkpoint>()->addCheckpoint(Vec3{-52.f, 5.f, -2.f});
        checkpoint2->getComponent<Checkpoint>()->addCheckpoint(Vec3{-15.f, 5.f, -4.f});
        checkpoint2->getComponent<Checkpoint>()->addCheckpoint(Vec3{-10.f, 5.f, -10.f});
        spawnerGO.addComponent<CircularEntitiesSpawner>(checkpoint2->getComponent<Checkpoint>(), 2.f, 0.5f, 0.f).setContenor(enemiesContener);
        spawnerGO.addComponent<Model>(modelSpawnerArg);
        _scene->add<GameObject>(spawnerGO, pointLightGameObjectArg).addComponent<PointLight>(lightArg);

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroyImmediate();
    }

    {
        spawnerGOArg.name = "Spawner3";
        spawnerGOArg.transformArg.position = {95.f, 5.f, 175.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        checkpoint3->addComponent<Checkpoint>().addCheckpoint(Vec3{52.f, 5.f, 170.f});
        checkpoint3->getComponent<Checkpoint>()->addCheckpoint(Vec3{30.f, 5.f, 110.f});
        checkpoint3->getComponent<Checkpoint>()->addCheckpoint(Vec3{5.f, 5.f, 35.f});
        checkpoint3->getComponent<Checkpoint>()->addCheckpoint(Vec3{-8.f, 5.f, -6.f});
        checkpoint3->getComponent<Checkpoint>()->addCheckpoint(Vec3{-10.f, 5.f, -10.f});
        spawnerGO.addComponent<CircularEntitiesSpawner>(checkpoint3->getComponent<Checkpoint>(), 2.f, 0.5f, 0.f).setContenor(enemiesContener);
        spawnerGO.addComponent<Model>(modelSpawnerArg);
        _scene->add<GameObject>(spawnerGO, pointLightGameObjectArg).addComponent<PointLight>(lightArg);

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroyImmediate();
    }

    {
        spawnerGOArg.name = "Spawner4";
        spawnerGOArg.transformArg.position = {108.f, 5.f, -173.f};

        GameObject& spawnerGO = _scene->add<GameObject>(_scene->getWorld(), spawnerGOArg);
        checkpoint4->addComponent<Checkpoint>().addCheckpoint(Vec3{135.f, 5.f, -109.f});
        checkpoint4->getComponent<Checkpoint>()->addCheckpoint(Vec3{114.f, 5.f, -60.f});
        checkpoint4->getComponent<Checkpoint>()->addCheckpoint(Vec3{90.f, 5.f, -21.f});
        checkpoint4->getComponent<Checkpoint>()->addCheckpoint(Vec3{35.f, 5.f, -15.f});
        checkpoint4->getComponent<Checkpoint>()->addCheckpoint(Vec3{-4.f, 5.f, -10.f});
        checkpoint4->getComponent<Checkpoint>()->addCheckpoint(Vec3{-10.f, 5.f, -10.f});
        spawnerGO.addComponent<CircularEntitiesSpawner>(checkpoint4->getComponent<Checkpoint>(), 2.f, 0.5f, 0.f).setContenor(enemiesContener);
        spawnerGO.addComponent<Model>(modelSpawnerArg);
        _scene->add<GameObject>(spawnerGO, pointLightGameObjectArg).addComponent<PointLight>(lightArg);

        Save::createPrefab(spawnerGO, spawnerGOArg.name);
        spawnerGO.destroyImmediate();
    }

    /*Create wave manager and assign spawner and enemies prefabs*/
    GameObjectCreateArg waveManagerArg {"waveManager"};

    GameObject& waveManagerGO = _scene->add<GameObject>(_scene->getWorld(), waveManagerArg);
    SpawnerPrefabs spawnerPrefs {"Spawner1", "Spawner2", "Spawner3", "Spawner4"};
    EnemiesPrefabs enemiesPrefs {"enemy1", "enemy2", "enemy3"};
    waveManagerGO.addComponent<WaveManager>(spawnerPrefs, enemiesPrefs, 0, 3).nextWave();
}

void Demo::loadBulletHoleContenor (int maxDecale)
{
    _scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"DecalContenor", {{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}}}).addComponent<MaxElementConteneur>(maxDecale);
}

void Demo::loadTimeManager        ()
{
    GameObject& timeManager = _scene->add<GameObject>(_scene->getWorld(), GameObjectCreateArg{"TimeManager"});
    timeManager.addComponent<DayNightCycle>(60.f, 120.f, 30.f, 180.f, 120.f, 30.f, 100.f, 1000.f);
}
