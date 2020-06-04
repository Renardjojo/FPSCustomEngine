#include "Game/UpgradeStation.hpp"
#include "Game/Firearm.hpp"

#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Ressources/scene.hpp"

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::System;
using namespace Engine::Core::Maths;

UpgradeStation::UpgradeStation(GameObject &gameObject, int cost)
    :   ScriptComponent    {gameObject},
        _cost{cost}
{
    _name = __FUNCTION__;
}

UpgradeStation::UpgradeStation (GameObject &refGameObject, const std::vector<std::string>& params)
    :   ScriptComponent     {refGameObject},
        _activationRadius   {std::stof(params[0])},
        _cost               {std::stoi(params[1])}
{
    _name = __FUNCTION__;
}

void UpgradeStation::start () noexcept
{
    _player = Scene::getCurrentScene()->getGameObject("world/Players/Player1").getComponent<PlayerController>();
}

void UpgradeStation::update () noexcept
{

}

void UpgradeStation::activate(Vec3 playerPosition, PlayerController* playerStats)
{
    (void)playerPosition;
    (void)playerStats;
}

void UpgradeStation::save(xml_document<>& doc, xml_node<>* nodeParent) 
{
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
    newNode->append_attribute(doc.allocate_attribute("activationRadius", doc.allocate_string(std::to_string(_activationRadius).c_str())));
    newNode->append_attribute(doc.allocate_attribute("cost", doc.allocate_string(std::to_string(_cost).c_str())));

    nodeParent->append_node(newNode);
}

void MunitionCapacityUpgradeStation::activate(Vec3 playerPosition, PlayerController* playerStats)
{
    if (*playerStats->getMoney() >= _cost)
    {
        if ((playerPosition - _gameObject.getGlobalPosition()).length() <= _activationRadius)
        {
            playerStats->getCurrentFirearm()->addMunitionCapacity(1);
            playerStats->getCurrentFirearm()->reload();
            playerStats->addMoney(-_cost);
        }
    }
}

void DamageUpgradeStation::activate(Vec3 playerPosition, PlayerController* playerStats)
{
    if (*playerStats->getMoney() >= _cost)
    {
        if ((playerPosition - _gameObject.getGlobalPosition()).length() <= _activationRadius)
        {
            playerStats->getCurrentFirearm()->addDamage(1);
            playerStats->addMoney(-_cost);
        }
    }
}

void FireRateUpgradeStation::activate(Vec3 playerPosition, PlayerController* playerStats)
{
    if (*playerStats->getMoney() >= _cost)
    {
        if ((playerPosition - _gameObject.getGlobalPosition()).length() <= _activationRadius)
        {
            playerStats->getCurrentFirearm()->setShotIntervalDelay(playerStats->getCurrentFirearm()->getShotIntervalDelay() / 2.f);
            playerStats->addMoney(-_cost);
        }
    }
}

void ReloadTimeUpgradeStation::activate(Vec3 playerPosition, PlayerController* playerStats)
{
    if (*playerStats->getMoney() >= _cost)
    {
        if ((playerPosition - _gameObject.getGlobalPosition()).length() <= _activationRadius)
        {
            playerStats->getCurrentFirearm()->setReloadTime(playerStats->getCurrentFirearm()->getReloadTime() / 2.f);
            playerStats->addMoney(-_cost);
        }
    }
}

void AutoUpgradeStation::activate(Vec3 playerPosition, PlayerController* playerStats)
{
    if (*playerStats->getMoney() >= _cost)
    {
        if ((playerPosition - _gameObject.getGlobalPosition()).length() <= _activationRadius)
        {
            if (playerStats->getCurrentFirearm()->getAutomaticState())
                return;
            
            playerStats->getCurrentFirearm()->setAutomaticState(true);
            playerStats->addMoney(-_cost);
        }
    }
}