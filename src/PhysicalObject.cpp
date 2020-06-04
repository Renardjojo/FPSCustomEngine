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

PhysicalObject::PhysicalObject(Engine::Ressources::GameObject &refGameObject, const PhysicalObjectCreateArg& arg)
    :   Component           {refGameObject},
        _velocity            {Vec3::zero},
        _angularVelocity     {Vec3::zero},
        _mass                {arg.mass},
        _freezeTrX           {arg.freezeTrX},
        _freezeTrY           {arg.freezeTrY},
        _freezeTrZ           {arg.freezeTrZ},
        _freezeRotX          {arg.freezeRotX},
        _freezeRotY          {arg.freezeRotY},
        _freezeRotZ          {arg.freezeRotZ},
        _isKinematic         {arg.isKinematic},
        _useGravity          {arg.useGravity},
        _isDirty             {false}
{
    _name = __FUNCTION__;
    if (_gameObject.getComponent<Collider>())
        _gameObject.getComponent<Collider>()->SetAttachedPhysicalObject(this);
    PhysicSystem::addPhysicalObject(this);
}

PhysicalObject::PhysicalObject (const PhysicalObject& other)
    :   Component            (other._gameObject),
        _velocity            (other._velocity),
        _angularVelocity     (other._angularVelocity),
        _mass                (other._mass),
        _freezeTrX           (other._freezeTrX),
        _freezeTrY           (other._freezeTrY),
        _freezeTrZ           (other._freezeTrZ),
        _freezeRotX          (other._freezeRotX),
        _freezeRotY          (other._freezeRotY),
        _freezeRotZ          (other._freezeRotZ),
        _isKinematic         (other._isKinematic),
        _useGravity          (other._useGravity),
        _isDirty             (other._isDirty)
{
    _name = __FUNCTION__;
    if (_gameObject.getComponent<Collider>())
        _gameObject.getComponent<Collider>()->SetAttachedPhysicalObject(this);
    PhysicSystem::addPhysicalObject(this);
}


PhysicalObject::PhysicalObject (PhysicalObject&& other)
    :   Component           (other._gameObject),
        _velocity            (std::move(other._velocity)),
        _angularVelocity     (std::move(other._angularVelocity)),
        _mass                (std::move(other._mass)),
        _freezeTrX           (std::move(other._freezeTrX)),
        _freezeTrY           (std::move(other._freezeTrY)),
        _freezeTrZ           (std::move(other._freezeTrZ)),
        _freezeRotX          (std::move(other._freezeRotX)),
        _freezeRotY          (std::move(other._freezeRotY)),
        _freezeRotZ          (std::move(other._freezeRotZ)),
        _isKinematic        (std::move(other._isKinematic)),
        _useGravity         (std::move(other._useGravity))
{
    _name = __FUNCTION__;
    if (_gameObject.getComponent<Collider>())
        _gameObject.getComponent<Collider>()->SetAttachedPhysicalObject(this);
    PhysicSystem::updatePhysicalObjectPointor(this, &other);
}

PhysicalObject::PhysicalObject (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
    :   Component            (refGameObject),
        _velocity            ({std::stof(params[0]), std::stof(params[1]), std::stof(params[2])}),
        _angularVelocity     ({std::stof(params[3]), std::stof(params[4]), std::stof(params[5])}),
        _mass                (std::stof(params[6])),
        _freezeTrX           (static_cast<bool>(std::stof(params[7]))),
        _freezeTrY           (static_cast<bool>(std::stof(params[8]))),
        _freezeTrZ           (static_cast<bool>(std::stof(params[9]))),
        _freezeRotX          (static_cast<bool>(std::stof(params[10]))),
        _freezeRotY          (static_cast<bool>(std::stof(params[11]))),
        _freezeRotZ          (static_cast<bool>(std::stof(params[12]))),
        _isKinematic         (static_cast<bool>(std::stof(params[13]))),
        _useGravity          (static_cast<bool>(std::stof(params[14]))),
        _isDirty             (static_cast<bool>(std::stof(params[15])))
{
    _name = __FUNCTION__;
    
    if (_gameObject.getComponent<Collider>())
        _gameObject.getComponent<Collider>()->SetAttachedPhysicalObject(this);
    PhysicSystem::addPhysicalObject(this);
}

PhysicalObject::~PhysicalObject ()
{
    PhysicSystem::removePhysicalObject(this);
}

void PhysicalObject::addForce(const Vec3& force) noexcept
{
    if (_isKinematic)
        return;

    _isDirty = true;

    if (!_freezeTrX)
        _velocity.x += force.x; 
    else
        _velocity.x = 0.f;

    if (!_freezeTrY)
        _velocity.y += force.y;
    else
        _velocity.y = 0.f;

    if (!_freezeTrZ)
        _velocity.z += force.z;
    else
        _velocity.z = 0.f;
}

void PhysicalObject::addForce(float x, float y, float z) noexcept
{
    if (_isKinematic)
        return;

    _isDirty = true;

    if (!_freezeTrX)
        _velocity.x += x;
    else
        _velocity.x = 0.f;
    
    if (!_freezeTrY)
        _velocity.y += y;
    else
        _velocity.y = 0.f;
    
    if (!_freezeTrZ)
        _velocity.z += z;
    else
        _velocity.z = 0.f;
}

void PhysicalObject::addTorque(const Vec3& force) noexcept
{
    if (_isKinematic)
        return;

    _isDirty = true;

    if (!_freezeRotX)
        _angularVelocity.x += force.x; 
    else
        _angularVelocity.x = 0.f;

    if (!_freezeRotY)
        _angularVelocity.y += force.y;
    else
        _velocity.y = 0.f;

    if (!_freezeRotZ)
        _angularVelocity.z += force.z;
    else
        _angularVelocity.z = 0.f;
}

void PhysicalObject::addTorque(float x, float y, float z) noexcept
{
    if (_isKinematic)
        return;
        
    _isDirty = true;

    if (!_freezeRotX)
        _angularVelocity.x += x;
    else
        _angularVelocity.x = 0.f;
    
    if (!_freezeRotY)
        _angularVelocity.y += y;
    else
        _angularVelocity.y = 0.f;
    
    if (!_freezeRotZ)
        _angularVelocity.z += z;
    else
        _angularVelocity.z = 0.f;
}

void PhysicalObject::setVelocity(const Engine::Core::Maths::Vec3& velocity) noexcept
{
    if (_isKinematic)
        return;

    _isDirty = true;
    _sleep = false;

    if (!_freezeTrX)
        _velocity.x = velocity.x; 
    else
        _velocity.x = 0.f;

    if (!_freezeTrY)
        _velocity.y = velocity.y;
    else
        _velocity.y = 0.f;

    if (!_freezeTrZ)
        _velocity.z = velocity.z;
    else
        _velocity.z = 0.f;
}

void PhysicalObject::setVelocity(float x, float y, float z) noexcept
{
    if (_isKinematic)
        return;

    _isDirty = true;
    _sleep = false;

    if (!_freezeTrX)
        _velocity.x = x; 
    else
        _velocity.x = 0.f;

    if (!_freezeTrY)
        _velocity.y = y;
    else
        _velocity.y = 0.f;

    if (!_freezeTrZ)
        _velocity.z = z;
    else
        _velocity.z = 0.f;
}

void PhysicalObject::setAngularVelocity(const Engine::Core::Maths::Vec3& velocity) noexcept
{
    if (_isKinematic)
        return;

    _isDirty = true;
    _sleep = false;

    if (!_freezeRotX)
        _angularVelocity.x = velocity.x; 
    else
        _angularVelocity.x = 0.f;

    if (!_freezeRotY)
        _angularVelocity.y = velocity.y;
    else
        _velocity.y = 0.f;

    if (!_freezeRotZ)
        _angularVelocity.z = velocity.z;
    else
        _angularVelocity.z = 0.f;

    _angularVelocity = velocity;
}

void PhysicalObject::addForceAtPoint(const Engine::Core::Maths::Vec3& force, const Engine::Core::Maths::Vec3& position) noexcept
{
    addForce    (force);
    addTorque   (Vec3::cross((position - _gameObject.getPosition()), force));
}

void PhysicalObject::save(xml_document<> &doc, xml_node<> *nodeParent) noexcept
{
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

    newNode->append_attribute(doc.allocate_attribute("velocity0", doc.allocate_string(std::to_string(_velocity.e[0]).c_str())));
    newNode->append_attribute(doc.allocate_attribute("velocity1", doc.allocate_string(std::to_string(_velocity.e[1]).c_str())));
    newNode->append_attribute(doc.allocate_attribute("velocity2", doc.allocate_string(std::to_string(_velocity.e[2]).c_str())));

    newNode->append_attribute(doc.allocate_attribute("AngularVelocity0", doc.allocate_string(std::to_string(_angularVelocity.e[0]).c_str())));
    newNode->append_attribute(doc.allocate_attribute("AngularVelocity1", doc.allocate_string(std::to_string(_angularVelocity.e[1]).c_str())));
    newNode->append_attribute(doc.allocate_attribute("AngularVelocity2", doc.allocate_string(std::to_string(_angularVelocity.e[2]).c_str())));

    newNode->append_attribute(doc.allocate_attribute("mass", doc.allocate_string(std::to_string(_mass).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeTrX", doc.allocate_string(std::to_string(_freezeTrX).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeTrY", doc.allocate_string(std::to_string(_freezeTrY).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeTrZ", doc.allocate_string(std::to_string(_freezeTrZ).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeRotX", doc.allocate_string(std::to_string(_freezeRotX).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeRotY", doc.allocate_string(std::to_string(_freezeRotY).c_str())));
    newNode->append_attribute(doc.allocate_attribute("freezeRotZ", doc.allocate_string(std::to_string(_freezeRotZ).c_str())));
    newNode->append_attribute(doc.allocate_attribute("isKinematic", doc.allocate_string(std::to_string(_isKinematic).c_str())));
    newNode->append_attribute(doc.allocate_attribute("useGravity", doc.allocate_string(std::to_string(_useGravity).c_str())));
    newNode->append_attribute(doc.allocate_attribute("isDirty", doc.allocate_string(std::to_string(_isDirty).c_str())));

    nodeParent->append_node(newNode);
}

#ifndef DNEDITOR
void PhysicalObject::serializeOnEditor () noexcept
{
    ImGui::Text("Mass :"); ImGui::SameLine(); ImGui::DragFloat("##mass", &_mass, 0.1f);

    ImGui::Text("Velocity :");
    ImGui::Columns(3);
    ImGui::Text("X :"); ImGui::SameLine(); ImGui::DragFloat("##velX", &_velocity.x, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Y :"); ImGui::SameLine(); ImGui::DragFloat("##velY", &_velocity.y, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Z :"); ImGui::SameLine(); ImGui::DragFloat("##velZ", &_velocity.z, 0.1f);
    ImGui::Columns(1);

    ImGui::Text("Torque :");
    ImGui::Columns(3);
    ImGui::Text("X :"); ImGui::SameLine(); ImGui::DragFloat("##TorqueX", &_angularVelocity.x, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Y :"); ImGui::SameLine(); ImGui::DragFloat("##TorqueY", &_angularVelocity.y, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Z :"); ImGui::SameLine(); ImGui::DragFloat("##TorqueZ", &_angularVelocity.z, 0.1f);
    ImGui::Columns(1);

    ImGui::Text("Freeze :");        
    ImGui::Checkbox("TrX", &_freezeTrX); ImGui::SameLine();
    ImGui::Checkbox("TrY", &_freezeTrY); ImGui::SameLine();
    ImGui::Checkbox("TrZ", &_freezeTrZ);

    ImGui::Checkbox("TqX", &_freezeRotX); ImGui::SameLine();
    ImGui::Checkbox("TqY", &_freezeRotY); ImGui::SameLine();
    ImGui::Checkbox("TqZ", &_freezeRotZ);

    ImGui::Text("Is Kinematic :"); ImGui::SameLine(); ImGui::Checkbox("##isKinematic", &_isKinematic); 
    ImGui::Text("Use gravity :"); ImGui::SameLine(); ImGui::Checkbox("##useGravity", &_useGravity); 
    ImGui::Text("Sleep :"); ImGui::SameLine(); ImGui::Checkbox("##sleep", &_sleep);
}
#endif