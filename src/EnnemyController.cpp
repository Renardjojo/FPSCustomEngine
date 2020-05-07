#include <iostream>
#include "Game/EnnemyController.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include <math.h>

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::InputSystem;
using namespace Engine::Core::Time;
using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;

EnnemyController::EnnemyController(GameObject &gameObject, GameObject *player) : ScriptComponent{gameObject},_player{player} {}
EnnemyController::~EnnemyController() {}

void EnnemyController::update()
{
    if (_player == nullptr)
        return;

    switch (_state)
    {
    case States::Idle:
        idle();
        break;

    case States::Chasing:
        chasing();
        break;
    }
}

void EnnemyController::idle()
{
    float length{(_player->entity->getPosition() - gameObject.entity.get()->getPosition()).length()};

    if (length > _exclusionRadius && length <= _radius)
        _state = States::Chasing;
}

void EnnemyController::chasing()
{

    Vec3 direction{_player->entity->getPosition() - gameObject.entity.get()->getPosition()};
    gameObject.entity.get()->translate(direction.getNormalize() * _speed * TimeSystem::getDeltaTime());

    float length{direction.length()};
    if (length > _radius || length <= _exclusionRadius)
        _state = States::Idle;
}