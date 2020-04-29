#include "GE/Collision&Physics/PhysicalObject.hpp"

using namespace Engine::Physics;

PhysicalObject::PhysicalObject ()
{

}

PhysicalObject::PhysicalObject (const PhysicalObject& other)
{

}

PhysicalObject::PhysicalObject (PhysicalObject&& other)
{

}

PhysicalObject::~PhysicalObject ()
{

}

PhysicalObject& PhysicalObject::operator=(PhysicalObject const&)
{

}

PhysicalObject& PhysicalObject::operator=(PhysicalObject &&)
{

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