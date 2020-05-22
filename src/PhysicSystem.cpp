#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/System/TimeSystem.hpp"

#include "GE/Ressources/GameObject.hpp"

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

#include <cmath>
#include <limits>

using namespace Engine::Ressources;
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

                        HitInfo hitInfo1{intersection, &collider2->getGameObject()};
                        HitInfo hitInfo2{intersection, &collider1->getGameObject()};

                        collider1->OnCollisionEnter(hitInfo1);
                        collider2->OnCollisionEnter(hitInfo2);
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
bool ifCollisionAffectPhysicalObject(bool collision, PhysicalObject* physicalObject, HitInfo& rayHitInfo)
{
    if (collision)
    {
        rayHitInfo.gameObject = &physicalObject->getGameObject();
        return true;
    }
    return false;
}

void keepNearestGameObject(GameObject*& currentGameObjectHit, float& firstIntersectionDistance, const Vec3& newIntersection, GameObject* newGameObjectHit, const Vec3& rayOrigine)
{
    float newIntersectionWithRayOrigineDist = (newIntersection - rayOrigine).length();
    
    if (newIntersectionWithRayOrigineDist < firstIntersectionDistance)
    {
        currentGameObjectHit = newGameObjectHit;
        firstIntersectionDistance = newIntersectionWithRayOrigineDist;
    }
}

bool PhysicSystem::rayCast(const Engine::Core::Maths::Shape3D::Segment& ray, HitInfo& rayHitInfo) noexcept
{
    bool collisionHappend               = false;
    GameObject* currentGameObjectHit    = nullptr;
    float firstIntersectionDistance     = std::numeric_limits<float>().max();

    for (Collider* collider : pColliders)
    {
        AABBCollider* aabbColliderPtr = dynamic_cast<AABBCollider*>(collider);
        if (aabbColliderPtr != nullptr)
        {
            if(SegmentAABB::isSegmentAABBCollided(ray, aabbColliderPtr->getGlobalAABB(), rayHitInfo.intersectionsInfo))
            {
                keepNearestGameObject(currentGameObjectHit, firstIntersectionDistance, rayHitInfo.intersectionsInfo.intersection1, &aabbColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        SphereCollider* sphereColliderPtr = dynamic_cast<SphereCollider*>(collider);
        if (sphereColliderPtr != nullptr)
        {
            if (SegmentSphere::isSegmentSphereCollided(ray, sphereColliderPtr->getGlobalSphere(), rayHitInfo.intersectionsInfo))
            {
                keepNearestGameObject(currentGameObjectHit, firstIntersectionDistance, rayHitInfo.intersectionsInfo.intersection1, &sphereColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        OrientedBoxCollider* orientedBoxColliderPtr = dynamic_cast<OrientedBoxCollider*>(collider);
        if (orientedBoxColliderPtr != nullptr)
        {
            if (SegmentOrientedBox::isSegmentOrientedBoxCollided(ray, orientedBoxColliderPtr->getGlobalOrientedBox(), rayHitInfo.intersectionsInfo))
            {
                keepNearestGameObject(currentGameObjectHit, firstIntersectionDistance, rayHitInfo.intersectionsInfo.intersection1, &orientedBoxColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        PlaneCollider* planeColliderPtr = dynamic_cast<PlaneCollider*>(collider);
        if (planeColliderPtr != nullptr)
        {
            if (SegmentPlane::isSegmentPlaneCollided(ray, planeColliderPtr->getGlobalPlane(), rayHitInfo.intersectionsInfo))
            {
                keepNearestGameObject(currentGameObjectHit, firstIntersectionDistance, rayHitInfo.intersectionsInfo.intersection1, &planeColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        QuadCollider* quadColliderPtr = dynamic_cast<QuadCollider*>(collider);
        if (quadColliderPtr != nullptr)
        {
            if (SegmentQuad::isSegmentQuadCollided(ray, quadColliderPtr->getGlobalQuad(), rayHitInfo.intersectionsInfo))
            {
                keepNearestGameObject(currentGameObjectHit, firstIntersectionDistance, rayHitInfo.intersectionsInfo.intersection1, &quadColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        CapsuleCollider* capsuleColliderPtr = dynamic_cast<CapsuleCollider*>(collider);
        if (capsuleColliderPtr != nullptr)
        {
            if (SegmentCapsule::isSegmentCapsuleCollided(ray, capsuleColliderPtr->getGlobalCapsule(), rayHitInfo.intersectionsInfo))
            {
                keepNearestGameObject(currentGameObjectHit, firstIntersectionDistance, rayHitInfo.intersectionsInfo.intersection1, &capsuleColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        InfiniteCylinderCollider* infiniteCylinderColliderPtr = dynamic_cast<InfiniteCylinderCollider*>(collider);
        if (infiniteCylinderColliderPtr != nullptr)
        {
            if (SegmentInfiniteCylinder::isSegmentInfiniteCylinderCollided(ray, infiniteCylinderColliderPtr->getGlobalInfiniteCylinder(), rayHitInfo.intersectionsInfo))
            {
                keepNearestGameObject(currentGameObjectHit, firstIntersectionDistance, rayHitInfo.intersectionsInfo.intersection1, &infiniteCylinderColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        CylinderCollider* cylinderColliderPtr = dynamic_cast<CylinderCollider*>(collider);
        if (cylinderColliderPtr != nullptr)
        {
            if (SegmentCylinder::isSegmentCylinderCollided(ray, cylinderColliderPtr->getGlobalCylinder(), rayHitInfo.intersectionsInfo))
            {
                keepNearestGameObject(currentGameObjectHit, firstIntersectionDistance, rayHitInfo.intersectionsInfo.intersection1, &cylinderColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }
    }

    rayHitInfo.gameObject = currentGameObjectHit;
    return collisionHappend;
}

bool PhysicSystem::rayCast(const Vec3& origin, const Vec3& direction, float maxDistance, HitInfo& rayHitInfo) noexcept
{
    return rayCast(Segment{origin, origin + maxDistance * direction}, rayHitInfo);
}

bool PhysicSystem::rayCast(const Vec3& pt1, const Vec3& pt2, HitInfo& rayHitInfo) noexcept
{
    return rayCast(Segment{pt1, pt2}, rayHitInfo);
}