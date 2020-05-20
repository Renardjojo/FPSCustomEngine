#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/System/TimeSystem.hpp"

#include "GE/Core/Maths/ShapeRelation/MovingSphereOrientedBox.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentAABB.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentCapsule.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentCylinder.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentInfiniteCylinder.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentOrientedBox.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentPlane.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentQuad.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentSegment.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentSphere.hpp"

#include "GE/Physics/ColliderShape/SphereCollider.hpp"
#include "GE/Physics/ColliderShape/AABBCollider.hpp"
#include "GE/Physics/ColliderShape/OrientedBoxCollider.hpp"
#include "GE/Physics/ColliderShape/CapsuleCollider.hpp"
#include "GE/Physics/ColliderShape/CylinderCollider.hpp"
#include "GE/Physics/ColliderShape/InifiteCylinderCollider.hpp"
#include "GE/Physics/ColliderShape/PlaneCollider.hpp"
#include "GE/Physics/ColliderShape/QuadCollider.hpp"

using namespace Engine::Physics;
using namespace Engine::Physics::ColliderShape;
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
                    Vec3 vectSpeed = (collider1->GetAttachedPhysicalObject()->GetVelocity() / collider1->GetAttachedPhysicalObject()->GetMass()) * TimeSystem::getFixedDeltaTime();

                    //std::cout << dynamic_cast<SphereCollider*>(collider1)->GetSphere().getGlobalSphere().getCenter()<< std::endl;
                    if (MovingSphereOrientedBox::isMovingSphereOrientedBoxCollided( 
                        dynamic_cast<SphereCollider*>(collider1)->getGlobalSphere(), dynamic_cast<OrientedBoxCollider*>(collider2)->getGlobalOrientedBox(), 
                        vectSpeed, intersection))
                    {
                        Vec3 OA = dynamic_cast<SphereCollider*>(collider1)->getGlobalSphere().getCenter();
                        Vec3 AB = OA + vectSpeed;

                        float t = (OA + AB - intersection.intersection1).length() / AB.length();

                        Vec3 velocityAtCollision = collider1->GetAttachedPhysicalObject()->GetVelocity() + gravity * collider1->GetAttachedPhysicalObject()->GetMass() * TimeSystem::getFixedDeltaTime() * (1.f - t);
                        Vec3 newVelocity = collider1->GetBounciness() * -(2.f * (velocityAtCollision.dotProduct(intersection.normalI1)) * intersection.normalI1 - velocityAtCollision);

                        collider1->getGameObject().setTranslation(intersection.intersection1 + (newVelocity / collider1->GetAttachedPhysicalObject()->GetMass()) * TimeSystem::getFixedDeltaTime());

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
        object->getGameObject().translate((object->GetVelocity() / object->GetMass()) * TimeSystem::getFixedDeltaTime());
    }
}

inline 
bool ifCollisionAffectPhysicalObject(bool collision, PhysicalObject* physicalObject, RayHitInfo& rayHitInfo)
{
    if (collision)
    {
        rayHitInfo.optionnalPhysicalObjectPtr = physicalObject;
        return true;
    }
    return false;
}

bool PhysicSystem::rayCast(const Engine::Core::Maths::Shape3D::Segment& ray, RayHitInfo& rayHitInfo) noexcept
{
    for (Collider* collider : pColliders)
    {
        if (!collider->GetAttachedPhysicalObject())
            continue;

        AABBCollider* aabbColliderPtr = dynamic_cast<AABBCollider*>(collider);
        if (aabbColliderPtr != nullptr)
        {
            if(SegmentAABB::isSegmentAABBCollided(ray, aabbColliderPtr->getGlobalAABB(), rayHitInfo.intersectionsInfo))
            {
                rayHitInfo.optionnalPhysicalObjectPtr = aabbColliderPtr->GetAttachedPhysicalObject();
                return true;
            }
            else 
            {
                continue;
            }
        }

        SphereCollider* sphereColliderPtr = dynamic_cast<SphereCollider*>(collider);
        if (sphereColliderPtr != nullptr)
        {
            if (SegmentSphere::isSegmentSphereCollided(ray, sphereColliderPtr->getGlobalSphere(), rayHitInfo.intersectionsInfo))
            {
                rayHitInfo.optionnalPhysicalObjectPtr = sphereColliderPtr->GetAttachedPhysicalObject();
                return true;
            }
            else 
            {
                continue;
            }
        }

        OrientedBoxCollider* orientedBoxColliderPtr = dynamic_cast<OrientedBoxCollider*>(collider);
        if (orientedBoxColliderPtr != nullptr)
        {
            if (SegmentOrientedBox::isSegmentOrientedBoxCollided(ray, orientedBoxColliderPtr->getGlobalOrientedBox(), rayHitInfo.intersectionsInfo))
            {
                rayHitInfo.optionnalPhysicalObjectPtr = orientedBoxColliderPtr->GetAttachedPhysicalObject();
                return true;
            }
            else 
            {
                continue;
            }
        }

        PlaneCollider* planeColliderPtr = dynamic_cast<PlaneCollider*>(collider);
        if (planeColliderPtr != nullptr)
        {
            if (SegmentPlane::isSegmentPlaneCollided(ray, planeColliderPtr->getGlobalPlane(), rayHitInfo.intersectionsInfo))
            {
                rayHitInfo.optionnalPhysicalObjectPtr = planeColliderPtr->GetAttachedPhysicalObject();
                return true;
            }
            else 
            {
                continue;
            }
        }

        QuadCollider* quadColliderPtr = dynamic_cast<QuadCollider*>(collider);
        if (quadColliderPtr != nullptr)
        {
            if (SegmentQuad::isSegmentQuadCollided(ray, quadColliderPtr->getGlobalQuad(), rayHitInfo.intersectionsInfo))
            {
                rayHitInfo.optionnalPhysicalObjectPtr = quadColliderPtr->GetAttachedPhysicalObject();
                return true;
            }
            else 
            {
                continue;
            }
        }

        CapsuleCollider* capsuleColliderPtr = dynamic_cast<CapsuleCollider*>(collider);
        if (capsuleColliderPtr != nullptr)
        {
            if (SegmentCapsule::isSegmentCapsuleCollided(ray, capsuleColliderPtr->getGlobalCapsule(), rayHitInfo.intersectionsInfo))
            {
                rayHitInfo.optionnalPhysicalObjectPtr = capsuleColliderPtr->GetAttachedPhysicalObject();
                return true;
            }
            else 
            {
                continue;
            }
        }

        InfiniteCylinderCollider* infiniteCylinderColliderPtr = dynamic_cast<InfiniteCylinderCollider*>(collider);
        if (infiniteCylinderColliderPtr != nullptr)
        {
            if (SegmentInfiniteCylinder::isSegmentInfiniteCylinderCollided(ray, infiniteCylinderColliderPtr->getGlobalInfiniteCylinder(), rayHitInfo.intersectionsInfo))
            {
                rayHitInfo.optionnalPhysicalObjectPtr = infiniteCylinderColliderPtr->GetAttachedPhysicalObject();
                return true;
            }
            else 
            {
                continue;
            }
        }

        CylinderCollider* cylinderColliderPtr = dynamic_cast<CylinderCollider*>(collider);
        if (cylinderColliderPtr != nullptr)
        {
            if (SegmentCylinder::isSegmentCylinderCollided(ray, cylinderColliderPtr->getGlobalCylinder(), rayHitInfo.intersectionsInfo))
            {
                rayHitInfo.optionnalPhysicalObjectPtr = cylinderColliderPtr->GetAttachedPhysicalObject();
                return true;
            }
            else 
            {
                continue;
            }
        }
    }

    return false;
}

bool PhysicSystem::rayCast(const Vec3& origin, const Vec3& direction, float maxDistance, RayHitInfo& rayHitInfo) noexcept
{
    return rayCast(Segment{origin, origin + maxDistance * direction}, rayHitInfo);
}

bool PhysicSystem::rayCast(const Vec3& pt1, const Vec3& pt2, RayHitInfo& rayHitInfo) noexcept
{
    return rayCast(Segment{pt1, pt2}, rayHitInfo);
}