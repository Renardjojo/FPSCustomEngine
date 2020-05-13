#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Core/Maths/ShapeRelation/MovingSphereOrientedBox.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"


using namespace Engine::Physics;
using namespace Engine::Core::System;
using namespace Engine::Core::Debug;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;

std::vector<PhysicalObject*>    PhysicSystem::pPhysicalObjects;
std::vector<Collider*>          PhysicSystem::pColliders;
Engine::Core::Maths::Vec3       PhysicSystem::gravity = {0,-9.81f,0};

void PhysicSystem::update() noexcept
{
    for (PhysicalObject* object : pPhysicalObjects)
    {
        if (!object || object->IsKinematic() || object->IsSleeping())
            continue;
        
        if (object->UseGravity())
            object->AddForce(gravity * object->GetMass() * TimeSystem::getFixedDeltaTime());
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
            // is kinematic todo

            if (collider1 != collider2)
            {
                if (!collider1->GetAttachedPhysicalObject())
                    continue;
                if (dynamic_cast<SphereCollider*>(collider1) && dynamic_cast<OrientedBoxCollider*>(collider2))
                {
                    Intersection intersection;
                    Vec3 vectSpeed = (collider1->GetAttachedPhysicalObject()->GetVelocity() / collider1->GetAttachedPhysicalObject()->GetMass()) * TimeSystem::getFixedUnscaledDeltaTime();

                    //std::cout << dynamic_cast<SphereCollider*>(collider1)->GetSphere().getGlobalSphere().getCenter()<< std::endl;
                    if (MovingSphereOrientedBox::isMovingSphereOrientedBoxCollided( 
                        dynamic_cast<SphereCollider*>(collider1)->GetSphere().getGlobalSphere(), dynamic_cast<OrientedBoxCollider*>(collider2)->GetBox().getGlobalOrientedBox(), 
                        vectSpeed, intersection))
                    {
                        Vec3 OA = dynamic_cast<SphereCollider*>(collider1)->GetSphere().getGlobalSphere().getCenter();
                        Vec3 AB = OA + vectSpeed;

                        float t = (OA + AB - intersection.intersection1).length() / AB.length();

                        Vec3 velocityAtCollision = collider1->GetAttachedPhysicalObject()->GetVelocity() + gravity * collider1->GetAttachedPhysicalObject()->GetMass() * TimeSystem::getFixedDeltaTime() * (1 - t);
                        Vec3 newVelocity = collider1->GetBounciness() * -(2 * (velocityAtCollision.dotProduct(intersection.normalI1)) * intersection.normalI1 - velocityAtCollision);

                        collider1->gameObject.entity->setTranslation(intersection.intersection1 + (newVelocity / collider1->GetAttachedPhysicalObject()->GetMass()) * TimeSystem::getFixedDeltaTime());

                        collider1->GetAttachedPhysicalObject()->SetVelocity(newVelocity);
                    }
                }
            }
        }
    }
    for (PhysicalObject* object : pPhysicalObjects)
    {
        if (!object || object->IsKinematic() || object->IsSleeping())
            continue;
        
        /*update movement induct by the differente force on the object*/
        object->getGameObject().entity->translate((object->GetVelocity() / object->GetMass()) * TimeSystem::getFixedDeltaTime());
    }
}