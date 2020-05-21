#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Ressources/Component.hpp"

#ifndef DNEDITOR
#include "imgui/imgui.h"
#endif

using namespace Engine::Physics;
using namespace Engine::Physics::ColliderShape;
using namespace Engine::Core::DataStructure;
using namespace Engine::Core::Maths;
using namespace Engine::Ressources;

PhysicalObject::PhysicalObject (GameObject& refGameObject)
    : Component(refGameObject)
{
    _name = __FUNCTION__;
    if (_gameObject.getComponent<Collider>())
        _gameObject.getComponent<Collider>()->SetAttachedPhysicalObject(this);
    PhysicSystem::addPhysicalObject(this);
}

PhysicalObject::PhysicalObject (const PhysicalObject& other)
    :   Component           (other._gameObject),
        velocity            (other.velocity),
        angularVelocity     (other.angularVelocity),
        mass                (other.mass),
        freezeTrX           (other.freezeTrX),
        freezeTrY           (other.freezeTrY),
        freezeTrZ           (other.freezeTrZ),
        freezeRotX          (other.freezeRotX),
        freezeRotY          (other.freezeRotY),
        freezeRotZ          (other.freezeRotZ),
        _isKinematic        (other._isKinematic),
        _useGravity         (other._useGravity)
{
    _name = __FUNCTION__;
    if (_gameObject.getComponent<Collider>())
        _gameObject.getComponent<Collider>()->SetAttachedPhysicalObject(this);
    PhysicSystem::addPhysicalObject(this);
}

PhysicalObject::PhysicalObject (PhysicalObject&& other)
    :   Component           (other._gameObject),
        velocity            (std::move(other.velocity)),
        angularVelocity     (std::move(other.angularVelocity)),
        mass                (std::move(other.mass)),
        freezeTrX           (std::move(other.freezeTrX)),
        freezeTrY           (std::move(other.freezeTrY)),
        freezeTrZ           (std::move(other.freezeTrZ)),
        freezeRotX          (std::move(other.freezeRotX)),
        freezeRotY          (std::move(other.freezeRotY)),
        freezeRotZ          (std::move(other.freezeRotZ)),
        _isKinematic        (std::move(other._isKinematic)),
        _useGravity         (std::move(other._useGravity))
{
    _name = __FUNCTION__;
    if (_gameObject.getComponent<Collider>())
        _gameObject.getComponent<Collider>()->SetAttachedPhysicalObject(this);
    PhysicSystem::updatePhysicalObjectPointor(this, &other);
}

PhysicalObject::~PhysicalObject ()
{
    PhysicSystem::removePhysicalObject(this);
}

void PhysicalObject::AddForce(Vec3 force)
{
    if (_isKinematic)
        return;

    _sleep = false;

    if (!freezeTrX)
        velocity.x += force.x; 
    else
        velocity.x = 0;

    if (!freezeTrY)
        velocity.y += force.y;
    else
        velocity.y = 0;

    if (!freezeTrZ)
        velocity.z += force.z;
    else
        velocity.z = 0;
}

void PhysicalObject::AddForce(float x, float y, float z)
{
    if (_isKinematic)
        return;

    _sleep = false;

    if (!freezeRotX)
        velocity.x += x;
    else
        velocity.x = 0;
    
    if (!freezeRotY)
        velocity.y += y;
    else
        velocity.y = 0;
    
    if (!freezeRotZ)
        velocity.z += z;
    else
        velocity.z = 0;
}

void PhysicalObject::AddTorque(Vec3 force)
{
    if (_isKinematic)
        return;
        
    if (!freezeRotX)
        angularVelocity.x += force.x; 
    else
        angularVelocity.x = 0;

    if (!freezeRotY)
        angularVelocity.y += force.y;
    else
        velocity.y = 0;

    if (!freezeRotZ)
        angularVelocity.z += force.z;
    else
        angularVelocity.z = 0;
}

void PhysicalObject::AddTorque(float x, float y, float z)
{
    if (_isKinematic)
        return;

    if (!freezeTrX)
        angularVelocity.x += x;
    else
        angularVelocity.x = 0;
    
    if (!freezeTrY)
        angularVelocity.y += y;
    else
        angularVelocity.y = 0;
    
    if (!freezeTrZ)
        angularVelocity.z += z;
    else
        angularVelocity.z = 0;
}
void PhysicalObject::save(xml_document<> &doc, xml_node<> *nodeParent)
{
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", "PhysicalObject"));

    nodeParent->append_node(newNode);
}

#ifndef DNEDITOR
void PhysicalObject::serializeOnEditor () noexcept
{
    ImGui::Text("Mass :"); ImGui::SameLine(); ImGui::DragFloat("##mass", &mass, 0.1f);

    ImGui::Text("Velocity :");
    ImGui::Columns(3);
    ImGui::Text("X :"); ImGui::SameLine(); ImGui::DragFloat("##velX", &velocity.x, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Y :"); ImGui::SameLine(); ImGui::DragFloat("##velY", &velocity.y, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Z :"); ImGui::SameLine(); ImGui::DragFloat("##velZ", &velocity.z, 0.1f);
    ImGui::Columns(1);

    ImGui::Text("Torque :");
    ImGui::Columns(3);
    ImGui::Text("X :"); ImGui::SameLine(); ImGui::DragFloat("##TorqueX", &angularVelocity.x, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Y :"); ImGui::SameLine(); ImGui::DragFloat("##TorqueY", &angularVelocity.y, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Z :"); ImGui::SameLine(); ImGui::DragFloat("##TorqueZ", &angularVelocity.z, 0.1f);
    ImGui::Columns(1);

    ImGui::Text("Freeze :");        
    ImGui::Checkbox("TrX", &freezeTrX); ImGui::SameLine();
    ImGui::Checkbox("TrY", &freezeTrY); ImGui::SameLine();
    ImGui::Checkbox("TrZ", &freezeTrZ);

    ImGui::Checkbox("TqX", &freezeRotX); ImGui::SameLine();
    ImGui::Checkbox("TqY", &freezeRotY); ImGui::SameLine();
    ImGui::Checkbox("TqZ", &freezeRotZ);

    ImGui::Text("Is Kinematic :"); ImGui::SameLine(); ImGui::Checkbox("##isKinematic", &_isKinematic); 
    ImGui::Text("Use gravity :"); ImGui::SameLine(); ImGui::Checkbox("##useGravity", &_useGravity); 
    ImGui::Text("Sleep :"); ImGui::SameLine(); ImGui::Checkbox("##sleep", &_sleep);
}
#endif