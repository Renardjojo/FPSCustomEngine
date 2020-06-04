#include "Game/LootMachine.hpp"

#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Physics/PhysicSystem.hpp"

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::System;
using namespace Engine::Core::Maths;

LootMachine::LootMachine(GameObject &gameObject)
    :   ScriptComponent    {gameObject}
{
    _name = __FUNCTION__;
}

LootMachine::LootMachine (GameObject &refGameObject, const std::vector<std::string>& params)
    :   ScriptComponent    {refGameObject},
        _forwardAnimDelay   {std::stof(params[0])},
        _delay              {std::stof(params[1])},
        _activationRadius   {std::stof(params[2])},
        _cost               {std::stoi(params[3])},
        _leverMoveForward   {static_cast<bool>(std::stoi(params[4]))},
        _isActivated        {static_cast<bool>(std::stoi(params[5]))}
{
    _name = __FUNCTION__;
}

void LootMachine::start () noexcept
{
    _leverGO = _gameObject.getChild("Lever");
    _spawner = _gameObject.getChild("Spawner")->getComponent<CircularEntitiesSpawner>();
}

void LootMachine::update () noexcept
{
    if (_isActivated)
    {
        if (_leverMoveForward)
        {
            _delay += TimeSystem::getDeltaTime();

            if (_delay >=_forwardAnimDelay)
            {
                _delay = _forwardAnimDelay - (_forwardAnimDelay - _delay);
                _leverMoveForward = false;

                _spawner->addEntitiesToSpawner(1, std::string("Lot") + std::to_string(Random::ranged<int>(1, 2)));
            }
        }
        else
        {
            _delay -= TimeSystem::getDeltaTime();

            if (_delay <= 0.f)
            {
                _delay = 0.f;
                _leverMoveForward = true;
                _isActivated = false;
            }
        }
        _leverGO->setRotation({_delay * static_cast<float>(M_PI_4) / _forwardAnimDelay, 0.f, 0.f});
    }
}

void LootMachine::activate(const Vec3& playerPosition, int& money)
{
    if (money >= _cost)
    {
        if ((playerPosition - _gameObject.getGlobalPosition()).length() <= _activationRadius)
        {
            _isActivated = true;
            money -= _cost;
        }
    }
}

void LootMachine::save(xml_document<>& doc, xml_node<>* nodeParent) 
{
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
    newNode->append_attribute(doc.allocate_attribute("forwardAnimDelay", doc.allocate_string(std::to_string(_forwardAnimDelay).c_str())));
    newNode->append_attribute(doc.allocate_attribute("delay", doc.allocate_string(std::to_string(_delay).c_str())));
    newNode->append_attribute(doc.allocate_attribute("activationRadius", doc.allocate_string(std::to_string(_activationRadius).c_str())));
    newNode->append_attribute(doc.allocate_attribute("cost", doc.allocate_string(std::to_string(_cost).c_str())));
    newNode->append_attribute(doc.allocate_attribute("leverMoveForward", doc.allocate_string(std::to_string(_leverMoveForward).c_str())));
    newNode->append_attribute(doc.allocate_attribute("isActivated", doc.allocate_string(std::to_string(_isActivated).c_str())));

    nodeParent->append_node(newNode);
}