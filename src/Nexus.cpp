//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-26 - 15 h 16

#include <iostream>
#include "Game/Nexus.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "Game/BarIndicatorController.hpp"
#include "GE/GE.hpp"
#include "Game/define.h"

using namespace Game;
using namespace Engine::Physics;
using namespace Engine::Physics::ColliderShape;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::InputSystem;
using namespace Engine::Core::System;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::ShapeRelation;
using namespace Engine::LowRenderer;

Nexus::Nexus(GameObject &_gameObject)
    : ScriptComponent{_gameObject}
{
}

Nexus::Nexus(GameObject &gameObject, const std::vector<std::string> &params)
    : ScriptComponent{gameObject},
    _life{std::stoi(params[0])},
    _maxLife {std::stoi(params[1])},
    _maxLightIntensity {std::stof(params[2])}
{}

void Nexus::start()
{
    _light = getGameObject().getComponent<PointLight>();
    GE_assertInfo(_light != nullptr, "no Nexus light");
    _gameObject.getChild("lifeBar")->getComponent<BarIndicatorController>()->init(_life, _maxLife);
    _maxLightIntensity = _light->getLinear();
}

void Nexus::update()
{
    if (_life <= 0)
    {
        _life = 0;
        Engine::GE::gameState = Engine::E_GAME_STATE::DEAD;
    }
}

void Nexus::fixedUpdate()
{

}

void Nexus::onCollisionEnter(HitInfo &hitInfo)
{
    (void)hitInfo;
}

void Nexus::inflictDamage(int damage)
{
    _life -= damage;
    _light->setLinear(_maxLightIntensity * static_cast<float>(_life) / static_cast<float>(_maxLife));
}

void Nexus::save(xml_document<> &doc, xml_node<> *nodeParent)
{
    if (!nodeParent)
        return;
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", "Nexus"));

    newNode->append_attribute(doc.allocate_attribute("life", std::to_string(_life).c_str()));
    newNode->append_attribute(doc.allocate_attribute("maxLife", std::to_string(_maxLife).c_str()));
    newNode->append_attribute(doc.allocate_attribute("maxLightIntensity", std::to_string(_maxLightIntensity).c_str()));

    nodeParent->append_node(newNode);
}