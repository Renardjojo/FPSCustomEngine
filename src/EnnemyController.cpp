#include <iostream>
#include "Game/EnnemyController.hpp"
#include "Game/PlayerController.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "Game/Nexus.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "Game/BarIndicatorController.hpp"
#include <math.h>


using namespace Game;
using namespace Engine::Physics;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::InputSystem;
using namespace Engine::Core::System;
using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;

EnnemyController::EnnemyController(GameObject &gameObject, GameObject *player, GameObject* nexus) 
:   ScriptComponent{gameObject},
    _player{player},
    _nexus{nexus}
{
    _name = __FUNCTION__;
}

void EnnemyController::setCheckpoint(Game::Checkpoint* checkpoint)
{
    _checkpointManager = CheckpointManager{checkpoint};
}

EnnemyController::EnnemyController(GameObject &gameObject, const std::vector<std::string> &params)
    : ScriptComponent{gameObject},
      _player{&Scene::getCurrentScene()->getGameObject(params[0])},
      _nexus{&Scene::getCurrentScene()->getGameObject(params[1])},
      _state{static_cast<States>(std::stoi(params[2]))},
      _checkpointManager{params[3].compare("nullptr") == 0 ? nullptr : Scene::getCurrentScene()->getGameObject(params[3]).getComponent<Checkpoint>(), 
      Vec3{std::stof(params[4]),std::stof(params[5]),std::stof(params[6])}, static_cast<unsigned int>(std::stoi(params[7]))},
      _radius{std::stof(params[8])},
      _attackRadius{std::stof(params[9])},
      _speed{std::stof(params[10])},
      _attackSpeed{std::stof(params[11])},
      _cooldown{std::stof(params[12])},
      _damage{std::stoi(params[13])},
      _life{std::stoi(params[14])}
{
    _name = __FUNCTION__;
}
void EnnemyController::start()
{
    _physics = _gameObject.getComponent<PhysicalObject>();
    _gameObject.getChild("lifeBar")->getComponent<BarIndicatorController>()->init(_life, _maxLife);
}

void EnnemyController::update()
{
    if (_life <= 0)
    {
        _gameObject.destroy();
        _player->getComponent<PlayerController>()->addMoney(_valueOnDeath);
    }

    switch (_state)
    {
        case States::Idle:
            idle();
            break;

        case States::Chasing:
            chasing();
            break;

        case States::Walking:
            walk();
            break;

        default:
            break;
    }
}

void EnnemyController::idle()
{
    float length{(_player->getPosition() - _gameObject.getPosition()).length()};

    if (length <= _radius)
        _state = States::Chasing;
    else
        _state = States::Walking;
}

void EnnemyController::walk()
{
    float length{(_player->getPosition() - _gameObject.getPosition()).length()};
    if (length < _radius)
        _state = States::Chasing;

    Vec3 direction{_checkpointManager._targetPos - _gameObject.getPosition()};
    direction.y = 0;
    _physics->addForce(direction.getNormalize() * _speed * TimeSystem::getDeltaTime());

    length = direction.length();
    if (length < 5.f)
        _checkpointManager.nextCheckPoint();

    if ((_nexus->getPosition() - _gameObject.getPosition()).length() < _attackRadius * 1.5f)
        autoDestroy();

}

void EnnemyController::chasing()
{
    _cooldown += TimeSystem::getDeltaTime();
    if (_player)
    {
        Vec3 direction{_player->getPosition() - _gameObject.getPosition()};
        direction.y = 0;
        _physics->addForce(direction.getNormalize() * _speed * TimeSystem::getDeltaTime());

        float length{direction.length()};
        if (_cooldown >= _attackSpeed && length < _attackRadius)
        {
           _player->getComponent<PlayerController>()->inflictDamage(_damage);
           _cooldown = 0.f;
        }
        if (length > _radius)
            _state = States::Walking;    
    }
}

void EnnemyController::autoDestroy()
{
    _nexus->getComponent<Nexus>()->inflictDamage(_damage);
    _gameObject.destroy();
}

void EnnemyController::onCollisionEnter(Engine::Physics::ColliderShape::HitInfo &hitInfo)
{
    if (hitInfo.gameObject->compareTag("Bullet"))
        _player->getComponent<PlayerController>()->addMoney(_valueOnHit);
}

void EnnemyController::save(xml_document<>& doc, xml_node<>* nodeParent)
{
    if (!nodeParent)
        return;
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

    newNode->append_attribute(doc.allocate_attribute("player", doc.allocate_string(_player->getRelativePath().c_str())));
    newNode->append_attribute(doc.allocate_attribute("nexus", doc.allocate_string(_nexus->getRelativePath().c_str())));

    newNode->append_attribute(doc.allocate_attribute("state", doc.allocate_string(std::to_string(_state).c_str())));

    newNode->append_attribute(doc.allocate_attribute("checkpointName", doc.allocate_string(_checkpointManager._checkpoints ? _checkpointManager._checkpoints->getGameObject().getRelativePath().c_str() : "nullptr")));
    newNode->append_attribute(doc.allocate_attribute("checkpointTargetX", doc.allocate_string(std::to_string(_checkpointManager._targetPos.x).c_str())));
    newNode->append_attribute(doc.allocate_attribute("checkpointTargetY", doc.allocate_string(std::to_string(_checkpointManager._targetPos.y).c_str())));
    newNode->append_attribute(doc.allocate_attribute("checkpointTargetZ", doc.allocate_string(std::to_string(_checkpointManager._targetPos.z).c_str())));
    newNode->append_attribute(doc.allocate_attribute("checkpointNumber", doc.allocate_string(std::to_string(_checkpointManager._cpNumber).c_str())));

    newNode->append_attribute(doc.allocate_attribute("radius", doc.allocate_string(std::to_string(_radius).c_str())));
    newNode->append_attribute(doc.allocate_attribute("attackRadius", doc.allocate_string(std::to_string(_attackRadius).c_str())));
    newNode->append_attribute(doc.allocate_attribute("speed", doc.allocate_string(std::to_string(_speed).c_str())));
    newNode->append_attribute(doc.allocate_attribute("attackSpeed", doc.allocate_string(std::to_string(_attackSpeed).c_str())));
    newNode->append_attribute(doc.allocate_attribute("cooldown", doc.allocate_string(std::to_string(_cooldown).c_str())));
    newNode->append_attribute(doc.allocate_attribute("damage", doc.allocate_string(std::to_string(_damage).c_str())));
    newNode->append_attribute(doc.allocate_attribute("life", doc.allocate_string(std::to_string(_life).c_str())));

    nodeParent->append_node(newNode);
}
