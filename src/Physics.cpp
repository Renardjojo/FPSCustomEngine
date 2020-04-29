#include "GE/Collision&Physics/Physics.hpp"

using namespace Engine::Physics;

std::vector<PhysicalObject*> Physic::pPhysicalObjects;
Engine::Core::Maths::Vec3 gravity;

void Physic::update()
{
    for (PhysicalObject* object : pPhysicalObjects)
    {
        if (object->isKinematic())
            continue;
            
        //object->translate((object->GetVelocity() / object->GetMass() + gravity) * deltaTime);

        // detect collision
    }
}