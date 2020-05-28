#include <iostream>
#include "Game/EnnemyController.hpp"
#include "Game/Nexus.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include <math.h>

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::InputSystem;
using namespace Engine::Core::System;
using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;

EnnemyController::EnnemyController(GameObject &gameObject, GameObject *player, Checkpoint* checkpoint) 
:   ScriptComponent{gameObject},
    _player{player},
    _checkpointManager{checkpoint}
{}

EnnemyController::EnnemyController(GameObject &gameObject, const std::vector<std::string> &params)
    : ScriptComponent{gameObject},
      _player{&Scene::getCurrentScene()->getGameObject(params[0])},
      _state{static_cast<States>(std::stoi(params[1]))},
      _checkpointManager{Scene::getCurrentScene()->getGameObject(params[2]).getComponent<Checkpoint>(), 
      Vec3{std::stof(params[3]),std::stof(params[4]),std::stof(params[5])}, static_cast<unsigned int>(std::stoi(params[6]))},
      _radius{std::stof(params[7])},
      _exclusionRadius{std::stof(params[8])},
      _speed{std::stof(params[9])},
      _attackSpeed{std::stof(params[10])},
      _cooldown{std::stof(params[11])},
      _damage{std::stoi(params[12])}
{
    
}

void EnnemyController::update()
{
    if (_player == nullptr)
    {
        walk();
        return;
    }

    switch (_state)
    {
    case States::Idle:
        idle();
        break;

    case States::Chasing:
        chasing();
        break;

    case States::Striking:
        break;

    case States::Walking:
        walk();
        break;
    }
}

void EnnemyController::idle()
{
    float length{(_player->getPosition() - _gameObject.getPosition()).length()};

    if (length > _exclusionRadius && length <= _radius)
        _state = States::Chasing;
}

void EnnemyController::walk()
{
    Vec3 direction{_checkpointManager._targetPos - _gameObject.getPosition()};
    _gameObject.translate(direction.getNormalize() * _speed * TimeSystem::getDeltaTime());

    float length{direction.length()};
    if (length < 0.1f)
        _checkpointManager.nextCheckPoint();

}

void EnnemyController::chasing()
{
    Vec3 direction{_player->getPosition() - _gameObject.getPosition()};
    _gameObject.translate(direction.getNormalize() * _speed * TimeSystem::getDeltaTime());

    float length{direction.length()};
    if (length > _radius || length <= _exclusionRadius)
        _state = States::Idle;
}

void EnnemyController::striking()
{
    _cooldown += TimeSystem::getDeltaTime();
   if (_nexus)
   {
        if (_cooldown >= _attackSpeed)
        {
           _nexus->getComponent<Nexus>()->InflictDamage(_damage);
           _cooldown = 0.f;
        }
   }
}

void EnnemyController::onCollisionEnter(Engine::Physics::ColliderShape::HitInfo &hitInfo)
{
    if (hitInfo.gameObject->CompareTag("Nexus"))
    {
        _state = Striking;
        _nexus = hitInfo.gameObject;
    }
}

void EnnemyController::save(xml_document<>& doc, xml_node<>* nodeParent)
{
    if (!nodeParent)
        return;
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", "EnnemyController"));

    newNode->append_attribute(doc.allocate_attribute("player", doc.allocate_string(_player->getRelativePath().c_str())));
    newNode->append_attribute(doc.allocate_attribute("state", doc.allocate_string(std::to_string(_state).c_str())));

    newNode->append_attribute(doc.allocate_attribute("checkpointName", doc.allocate_string(_checkpointManager._checkpoints->getGameObject().getRelativePath().c_str())));
    newNode->append_attribute(doc.allocate_attribute("checkpointTargetX", doc.allocate_string(std::to_string(_checkpointManager._targetPos.x).c_str())));
    newNode->append_attribute(doc.allocate_attribute("checkpointTargetY", doc.allocate_string(std::to_string(_checkpointManager._targetPos.y).c_str())));
    newNode->append_attribute(doc.allocate_attribute("checkpointTargetZ", doc.allocate_string(std::to_string(_checkpointManager._targetPos.z).c_str())));
    newNode->append_attribute(doc.allocate_attribute("checkpointNumber", doc.allocate_string(std::to_string(_checkpointManager._cpNumber).c_str())));

    newNode->append_attribute(doc.allocate_attribute("radius", doc.allocate_string(std::to_string(_radius).c_str())));
    newNode->append_attribute(doc.allocate_attribute("exclusionRadius", doc.allocate_string(std::to_string(_exclusionRadius).c_str())));
    newNode->append_attribute(doc.allocate_attribute("speed", doc.allocate_string(std::to_string(_speed).c_str())));
    newNode->append_attribute(doc.allocate_attribute("attackSpeed", doc.allocate_string(std::to_string(_attackSpeed).c_str())));
    newNode->append_attribute(doc.allocate_attribute("cooldown", doc.allocate_string(std::to_string(_cooldown).c_str())));
    newNode->append_attribute(doc.allocate_attribute("damage", doc.allocate_string(std::to_string(_damage).c_str())));

    nodeParent->append_node(newNode);
}
