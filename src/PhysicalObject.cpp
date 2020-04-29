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

}

PhysicalObject& PhysicalObject::operator=(PhysicalObject && other)
{
    Physic::updatePhysicalObjectPointor(this, &other);
}
