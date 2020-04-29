#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/PhysicSystem.hpp"

using namespace Engine::Physics;

PhysicalObject::PhysicalObject (Engine::Core::DataStructure::GraphEntity& refGameObject)
    : gameObject    (refGameObject)
{
    PhysicSystem::addPhysicalObject(this);
}

PhysicalObject::PhysicalObject (const PhysicalObject& other)
    :   velocity            (other.velocity),
        angularVelocity     (other.angularVelocity),
        mass                (other.mass),
        freezeTrX           (other.freezeTrX),
        freezeTrY           (other.freezeTrY),
        freezeTrZ           (other.freezeTrZ),
        freezeRotX          (other.freezeRotX),
        freezeRotY          (other.freezeRotY),
        freezeRotZ          (other.freezeRotZ),
        _isKinematic        (other._isKinematic),
        _useGravity         (other._useGravity),
        gameObject          (other.gameObject)
{
    PhysicSystem::addPhysicalObject(this);
}

PhysicalObject::PhysicalObject (PhysicalObject&& other)
    :   velocity            (std::move(other.velocity)),
        angularVelocity     (std::move(other.angularVelocity)),
        mass                (std::move(other.mass)),
        freezeTrX           (std::move(other.freezeTrX)),
        freezeTrY           (std::move(other.freezeTrY)),
        freezeTrZ           (std::move(other.freezeTrZ)),
        freezeRotX          (std::move(other.freezeRotX)),
        freezeRotY          (std::move(other.freezeRotY)),
        freezeRotZ          (std::move(other.freezeRotZ)),
        _isKinematic        (std::move(other._isKinematic)),
        _useGravity         (std::move(other._useGravity)),
        gameObject          (other.gameObject)
{
    PhysicSystem::updatePhysicalObjectPointor(this, &other);
}

PhysicalObject::~PhysicalObject ()
{
    PhysicSystem::removePhysicalObject(this);
}

void PhysicalObject::AddForce(Vec3 force)
{
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
    if (!freezeTrX)
        velocity.x += x;
    else
        velocity.x = 0;
    
    if (!freezeTrY)
        velocity.y += y;
    else
        velocity.x = 0;
    
    if (!freezeTrZ)
        velocity.z += z;
    else
        velocity.x = 0;
}