#include "GE/Physics/PhysicSystem.hpp"

using namespace Engine::Physics;

std::vector<PhysicalObject*>    PhysicSystem::pPhysicalObjects;
Engine::Core::Maths::Vec3       PhysicSystem::gravity;

void PhysicSystem::update()
{
    /*for (PhysicalObject* object : pPhysicalObjects)
    {
        //object->translate((object->GetVelocity() / object->GetMass() + gravity) * deltaTime);

        // detect collision
    }*/
}