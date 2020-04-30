#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Core/TimeSystem/time.hpp"

using namespace Engine::Physics;
using namespace Engine::Core::Time;

std::vector<PhysicalObject*>    PhysicSystem::pPhysicalObjects;
Engine::Core::Maths::Vec3       PhysicSystem::gravity = {0,-9.81f,0};

void PhysicSystem::update() noexcept
{
    for (PhysicalObject* object : pPhysicalObjects)
    {
        if (object->IsKinematic())
            continue;

        if (object->UseGravity())
            object->AddForce(gravity * TimeSystem::getDeltaTime());
        
        /*update movement induct by the differente force on the object*/
        object->getGameObject().entity->translate((object->GetVelocity() / object->GetMass()) * TimeSystem::getDeltaTime());
    }

    for (Collider* collider1 : pColliders)
    {
        if (collider1->GetAttachedPhysicalObject()->IsKinematic())
            continue;

        for (Collider* collider2 : pColliders)
        {
            if (collider2->GetAttachedPhysicalObject()->IsKinematic())
                continue;

            if (collider1 != collider2)
            {
                if (collider1->isCollidingWith(*collider2))
                { // Colliding
                    collider1->GetAttachedPhysicalObject()->SetVelocity(collider1->GetAttachedPhysicalObject()->GetVelocity()); 
                    collider2->GetAttachedPhysicalObject()->SetVelocity(collider2->GetAttachedPhysicalObject()->GetVelocity()); 
                }
            }
        }
    }
}