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
        material.push_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "PinkMaterial";
    matDefault.comp.ambient.rgbi = Vec4{1.f, 0.f, 1.f, 1.f};

    {
        std::vector<Material> material;
        material.push_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "RedMaterial";
    matDefault.comp.ambient.rgbi = Vec4{1.f, 0.f, 0.f, 1.f};
    {
        std::vector<Material> material;
        material.push_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "GreenMaterial";
    matDefault.comp.ambient.rgbi = Vec4{0.f, 1.f, 0.f, 1.f};

    {
        std::vector<Material> material;
        material.push_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "CyanMaterial";
    matDefault.comp.ambient.rgbi = Vec4{0.f, 1.f, 1.f, 1.f};

    {
        std::vector<Material> material;
        material.push_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "BlueMaterial";
    matDefault.comp.ambient.rgbi = Vec4{0.f, 0.f, 1.f, 1.f};

    {
        std::vector<Material> material;
        material.push_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "BlackMaterial";
    matDefault.comp.ambient.rgbi = Vec4{0.f, 0.f, 0.f, 1.f};

    {
        std::vector<Material> material;
        material.push_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "BrownMaterial";
    matDefault.comp.ambient.rgbi = Vec4{0.4f, 0.2f, 0.f, 1.f};

    {
        std::vector<Material> material;
        material.push_back(matDefault);
        ressourceManager.add<std::vector<Material>>(matDefault.name, std::move(material));
    }

    matDefault.name = "YellowMaterial";
    matDefault.comp.ambient.rgbi = Vec4{1.f, 1.f, 0.f, 1.f};

    {
        std::vector<Material> material;
        material.push_back(matDefault);
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

    loadSounds(_gameEngine.ressourceManager_);
    loadRockRessource           (ressourceManager);
    loadTreeRessource           (ressourceManager);
    loadSkyboxRessource         (ressourceManager);
    loadGunRessource            (ressourceManager);
    loadPseudoRessource         (ressourceManager);
    loadNexusRessource          (ressourceManager);  
    loadGroundRessource         (ressourceManager);
    loadEnemiesRessource        (ressourceManager);  
    loadCrateRessource          (ressourceManager);  
    loadGlassRessource          (ressourceManager);
    loadFogRessource            (ressourceManager);
    loadLootRessource           (ressourceManager); 
    loadSniperScopeRessource    (ressourceManager);
    loadUpgradeStationRessource (ressourceManager);

    TextureCreateArg tcaCrosshair{"./ressources/texture/crossair.png",E_WrapType::CLAMP_TO_BORDER};
    TextureCreateArg tcaBullet{"./ressources/texture/Bullet.png",E_WrapType::CLAMP_TO_BORDER,E_FilterType::LINEAR,false};
    TextureCreateArg tcaHealth{"./ressources/texture/Health.png",E_WrapType::CLAMP_TO_BORDER};
    TextureCreateArg hitmarkerTextureArg{"./ressources/texture/hitmarker.png",E_WrapType::CLAMP_TO_BORDER};

    ressourceManager.add<Texture>("crosshair", tcaCrosshair);
    ressourceManager.add<Texture>("bullet", tcaBullet);
    ressourceManager.add<Texture>("Health", tcaHealth);
    ressourceManager.add<Texture>("hitmarker", hitmarkerTextureArg);
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
    rm.add<Sound>("Hit","./ressources/sound/Hit.wav").setGain(_effectsVolume * 3.f);

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
    fontArg.fontRoot = "./ressources/font/opensans.ttf";
    fontArg.fontSize = 10;

    Font &font = ressourceManager.add<Font>("opensansPseudo", fontArg);

    TextCreateArg textArg;
    textArg.text = "Press F";
    textArg.pFont = &font;
    textArg.color = {0.f, 0.3f, 8.f, 1.f};
    textArg.wrapType = E_WrapType::CLAMP_TO_BORDER;

    MaterialCreateArg matText;
    matText.name = "IndicatorTextMaterial";
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

void Demo::loadGroundRessource(t_RessourcesManager &ressourceManager)
{
    ressourceManager.add<Mesh>("GroundMesh" ,Mesh::createPlane(300));

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

void Demo::loadEnemiesRessource(t_RessourcesManager &ressourceManager)
{
    {
        Attrib                      attrib;
        std::vector<Shape>          shape;
        std::vector<MaterialAttrib> materialAttribs;

        loadObjWithMTL("./ressources/obj/eyeball.obj", &attrib, &shape, &materialAttribs);
        ressourceManager.add<Mesh>("EyesBallMesh" , attrib, shape);

        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign(materialAttribs.begin(), materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("EyesBallMaterial", std::move(material));
        
    }

    {
        Attrib                      attrib;
        std::vector<Shape>          shape;
        std::vector<MaterialAttrib> materialAttribs;

        loadObjWithMTL("./ressources/obj/spider.obj", &attrib, &shape, &materialAttribs);

        ressourceManager.add<Mesh>("SpiderMesh" , attrib, shape);

        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign(materialAttribs.begin(), materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("SpiderMaterial", std::move(material));
    }

    {
        Attrib                      attrib;
        std::vector<Shape>          shape;
        std::vector<MaterialAttrib> materialAttribs;

        loadObjWithMTL("./ressources/obj/plantMonster.obj", &attrib, &shape, &materialAttribs);

        ressourceManager.add<Mesh>("PlantMonsterMesh" , attrib, shape);

        std::vector<Material> material;
        material.reserve(materialAttribs.size());
        /*Instanciate material vector with data read on materalAtribs*/
        material.assign(materialAttribs.begin(), materialAttribs.end());
        ressourceManager.add<std::vector<Material>>("PlantMonsterMaterial", std::move(material));
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
    matNexus.pathDiffuseTexture = "./ressources/texture/Nexus.png";

    {
        std::vector<Material> material;
        material.emplace_back(matNexus);
        ressourceManager.add<std::vector<Material>>(matNexus.name, std::move(material));
    }
}

void Demo::loadGlassRessource(t_RessourcesManager& ressourceManager)
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

void Demo::loadFogRessource(t_RessourcesManager& ressourceManager)
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

void Demo::loadLootRessource(t_RessourcesManager& ressourceManager)
{
    MaterialAndTextureCreateArg matCrate;
    matCrate.name = "RedCrateMaterial";
    matCrate.comp.ambient.rgbi = {1.f, 0.3f, 0.3f, 1.f};
    matCrate.comp.diffuse.rgbi = {1.f, 0.3f, 0.3f, 1.f};
    matCrate.comp.specular.rgbi = {1.f, 0.3f, 0.3f, 0.1f};
    matCrate.pathDiffuseTexture = "./ressources/texture/crate.png";

    {
        std::vector<Material> material;
        material.emplace_back(matCrate);
        ressourceManager.add<std::vector<Material>>(matCrate.name, std::move(material));
    }

    matCrate.name = "BlueCrateMaterial";
    matCrate.comp.ambient.rgbi = {0.3f, 0.3f, 1.f, 1.f};
    matCrate.comp.diffuse.rgbi = {0.3f, 0.3f, 1.f, 1.f};
    matCrate.comp.specular.rgbi = {0.3f, 0.3f, 1.f, 0.1f};

    {
        std::vector<Material> material;
        material.emplace_back(matCrate);
        ressourceManager.add<std::vector<Material>>(matCrate.name, std::move(material));
    }

    matCrate.name = "GreenCrateMaterial";
    matCrate.comp.ambient.rgbi = {0.3f, 1.f, 0.3f, 1.f};
    matCrate.comp.diffuse.rgbi = {0.3f, 1.f, 0.3f, 1.f};
    matCrate.comp.specular.rgbi = {0.3f, 1.f, 0.3f, 0.1f};

    {
        std::vector<Material> material;
        material.emplace_back(matCrate);
        ressourceManager.add<std::vector<Material>>(matCrate.name, std::move(material));
    }

    matCrate.name = "YellowCrateMaterial";
    matCrate.comp.ambient.rgbi = {1.f, 1.f, 0.3f, 1.f};
    matCrate.comp.diffuse.rgbi = {1.f, 1.f, 0.3f, 1.f};
    matCrate.comp.specular.rgbi = {1.f, 1.f, 0.3f, 0.1f};

    {
        std::vector<Material> material;
        material.emplace_back(matCrate);
        ressourceManager.add<std::vector<Material>>(matCrate.name, std::move(material));
    }
}
void Demo::loadSniperScopeRessource (t_RessourcesManager& ressourceManager)
{
    TextureCreateArg tcaSniperScope{"./ressources/texture/sniperScope.png",E_WrapType::CLAMP_TO_BORDER,E_FilterType::LINEAR,false};
    Texture &sniperScopeTexture = ressourceManager.add<Texture>("sniperScopeTexture", tcaSniperScope);
    
    ressourceManager.add<Shader>("ImageShader","./ressources/shader/text.vs","./ressources/shader/texture.fs");

    ressourceManager.add<Image>("SniperScope",
                                sniperScopeTexture.getID(),
                                ressourceManager.get<Shader>("ImageShader"),
                                0, 0,
                                _gameEngine.getWinSize().width,
                                _gameEngine.getWinSize().height,
                                E_GAME_STATE::RUNNING).isActive = false;;
}


void Demo::loadUpgradeStationRessource(t_RessourcesManager& ressourceManager)
{
    MaterialAndTextureCreateArg frontMat;
    frontMat.name = "ReloadMaterial";
    frontMat.comp.ambient.rgbi = {1.f, 1.f, 1.f, 1.f};
    frontMat.comp.diffuse.rgbi = {1.f, 1.f, 1.f, 1.f};
    frontMat.comp.specular.rgbi = {1.f, 1.f, 1.f, 0.1f};
    frontMat.pathDiffuseTexture = "./ressources/texture/reload.png";
    {
        std::vector<Material> material;
        material.emplace_back(frontMat);
        ressourceManager.add<std::vector<Material>>(frontMat.name, std::move(material));
    }

    frontMat.name = "FireRateMaterial";
    frontMat.pathDiffuseTexture = "./ressources/texture/firerate.png";
    {
        std::vector<Material> material;
        material.emplace_back(frontMat);
        ressourceManager.add<std::vector<Material>>(frontMat.name, std::move(material));
    }

    frontMat.name = "DamageMaterial";
    frontMat.pathDiffuseTexture = "./ressources/texture/damage.png";
    {
        std::vector<Material> material;
        material.emplace_back(frontMat);
        ressourceManager.add<std::vector<Material>>(frontMat.name, std::move(material));
    }

    frontMat.name = "AutoMaterial";
    frontMat.pathDiffuseTexture = "./ressources/texture/auto.png";
    {
        std::vector<Material> material;
        material.emplace_back(frontMat);
        ressourceManager.add<std::vector<Material>>(frontMat.name, std::move(material));
    }

    frontMat.name = "MunitionMaterial";
    frontMat.pathDiffuseTexture = "./ressources/texture/munition.png";
    {
        std::vector<Material> material;
        material.emplace_back(frontMat);
        ressourceManager.add<std::vector<Material>>(frontMat.name, std::move(material));
    }
}