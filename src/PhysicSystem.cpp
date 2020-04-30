#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Core/TimeSystem/time.hpp"
#include "GE/Core/Debug/log.hpp"

using namespace Engine::Physics;
using namespace Engine::Core::Time;
using namespace Engine::Core::Debug;

std::vector<PhysicalObject*>    PhysicSystem::pPhysicalObjects;
std::vector<Collider*>          PhysicSystem::pColliders;
Engine::Core::Maths::Vec3       PhysicSystem::gravity = {0,-9.81f,0};

void PhysicSystem::update() noexcept
{
    for (PhysicalObject* object : pPhysicalObjects)
    {
        if (!object || object->IsKinematic())
            continue;

        if (object->UseGravity())
            object->AddForce(gravity * object->GetMass() * TimeSystem::getDeltaTime());
        
        /*update movement induct by the differente force on the object*/
        object->getGameObject().entity->translate((object->GetVelocity() / object->GetMass()) * TimeSystem::getDeltaTime());
    }

    for (Collider* collider1 : pColliders)
    {

        // if (collider1->GetAttachedPhysicalObject())
        // {
        //     if (collider1->GetAttachedPhysicalObject()->IsKinematic())
        //         continue;
        // }

        for (Collider* collider2 : pColliders)
        {
            static_cast<SphereCollider*>(collider1)->GetSphere().setCenter(collider1->gameObject.entity->getPosition());
            // is kinematic
            if (collider1 != collider2)
            {
                if (collider1->isCollidingWith(*collider2))
                { // Colliding

                    if (collider1->GetAttachedPhysicalObject())
                        collider1->GetAttachedPhysicalObject()->SetVelocity(-collider1->GetAttachedPhysicalObject()->GetVelocity());
                    if (collider2->GetAttachedPhysicalObject())
                        collider2->GetAttachedPhysicalObject()->SetVelocity(collider2->GetAttachedPhysicalObject()->GetVelocity()); 
                }
            }
        }
    }
}