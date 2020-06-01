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
    _life{std::stof(params[0])}
{}

void Nexus::start()
{
    _life = 10;
}

void Nexus::update()
{
    if (_life <= 0)
        std::cout << "nexus destroy" << std::endl;
}

void Nexus::fixedUpdate()
{
}

void Nexus::onCollisionEnter(HitInfo& hitInfo)
{
    (void)hitInfo;
}

void Nexus::save(xml_document<>& doc, xml_node<>* nodeParent)
{
    if (!nodeParent)
        return;
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", "Nexus"));

    newNode->append_attribute(doc.allocate_attribute("life", std::to_string(_life).c_str()));

    nodeParent->append_node(newNode);
}