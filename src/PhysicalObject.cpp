#include "GE/Collision&Physics/PhysicalObject.hpp"
#include "GE/Collision&Physics/PhysicSystem.hpp"

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
