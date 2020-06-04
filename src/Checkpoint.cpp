//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-26 - 15 h 50

#include <iostream>
#include "Game/Checkpoint.hpp"
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

Checkpoint::Checkpoint(GameObject &_gameObject)
    : ScriptComponent{_gameObject}
{
    _name = __FUNCTION__;
}

void Checkpoint::start()
{
}

void Checkpoint::update()
{
}

void Checkpoint::fixedUpdate()
{
}

void Checkpoint::addCheckpoint(Engine::Core::Maths::Vec3 newPos) 
{
    checkpoints.push_back(newPos);
}

Engine::Core::Maths::Vec3 Checkpoint::getCheckpoint(unsigned int cpNumber)
{
    if (checkpoints.size() == 0)
        return Vec3{0, 0, 0};
    else if (cpNumber > checkpoints.size() - 1)
        return checkpoints.back();
    else
        return checkpoints[cpNumber];
}

void Checkpoint::onCollisionEnter(HitInfo &hitInfo)
{
    (void)hitInfo;
}

void Checkpoint::save(xml_document<>& doc, xml_node<>* nodeParent)
{
    if (!nodeParent)
        return;
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", "Checkpoint"));

    nodeParent->append_node(newNode);
}
