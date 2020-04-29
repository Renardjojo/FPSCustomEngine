#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/PhysicSystem.hpp"

using namespace Engine::Physics;

PhysicalObject::PhysicalObject ()
{
    PhysicSystem::addPhysicalObject(this);
}

PhysicalObject::PhysicalObject (const PhysicalObject& other)
{
    PhysicSystem::addPhysicalObject(this);
}

PhysicalObject::PhysicalObject (PhysicalObject&& other)
{
    PhysicSystem::updatePhysicalObjectPointor(this, &other);
}

PhysicalObject::~PhysicalObject ()
{
    PhysicSystem::removePhysicalObject(this);
}

PhysicalObject& PhysicalObject::operator=(PhysicalObject const& other)
{
    PhysicSystem::addPhysicalObject(this);
}

PhysicalObject& PhysicalObject::operator=(PhysicalObject && other)
{
    PhysicSystem::updatePhysicalObjectPointor(this, &other);
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