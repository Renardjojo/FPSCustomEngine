#include "GE/Collision&Physics/PhysicalObject.hpp"
#include "GE/Collision&Physics/Physics.hpp"

using namespace Engine::Physics;

PhysicalObject::PhysicalObject ()
{
    Physic::addPhysicalObject(this);
}

PhysicalObject::PhysicalObject (const PhysicalObject& other)
{
    Physic::addPhysicalObject(this);
}

PhysicalObject::PhysicalObject (PhysicalObject&& other)
{
    Physic::updatePhysicalObjectPointor(this, &other);
}

PhysicalObject::~PhysicalObject ()
{
    Physic::removePhysicalObject(this);
}

PhysicalObject& PhysicalObject::operator=(PhysicalObject const& other)
{
    Physic::addPhysicalObject(this);
}

PhysicalObject& PhysicalObject::operator=(PhysicalObject && other)
{
    Physic::updatePhysicalObjectPointor(this, &other);
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