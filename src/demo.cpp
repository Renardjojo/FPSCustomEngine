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
    loadLights();
    loadBulletHoleContenor (50);
    loadEnemies(_gameEngine.ressourceManager_);
    loadUI(_gameEngine.ressourceManager_);
    loadUpgradeStation(_gameEngine.ressourceManager_);

    ScriptSystem::start();

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
        glViewport(0, 0, sizeWin.width, sizeWin.height);

        RenderingSystem::draw();
    }

    UISystem::draw(_gameEngine);

    glUseProgram(0);
}

void Demo::updateControl()
{
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