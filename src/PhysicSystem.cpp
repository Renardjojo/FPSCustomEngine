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

std::vector<PhysicalObject *> PhysicSystem::pPhysicalObjects;
std::vector<Collider *> PhysicSystem::pColliders;
Vec3 PhysicSystem::gravity = {0, -9.81f, 0};

struct CollisionTempInfo
{
    SphereCollider * collider        {nullptr};
    PhysicalObject * physicalObject  {nullptr};

    Vec3 position                   {Vec3::zero};
    Vec3 newVelocity                {Vec3::zero};
    Vec3 newAngularVelocity         {Vec3::zero};
    Vec3 currentVelocity            {Vec3::zero};
};

void keepNearestGameObject(GameObject *& currentGameObjectHit,
                           Intersection &currentIntersection,
                           float &firstIntersectionDistance,
                           const Intersection &newIntersection,
                           GameObject *newGameObjectHit,
                           const Vec3 &entityOrigin)
{
    float newIntersectionWithEntityOrigineDist = (newIntersection.intersection1 - entityOrigin).length();

    if (newIntersectionWithEntityOrigineDist < firstIntersectionDistance)
    {
        currentGameObjectHit = newGameObjectHit;
        firstIntersectionDistance = newIntersectionWithEntityOrigineDist;
        currentIntersection = newIntersection;
    }
}

void checkCollisionBetweenMovingSphereAndStaticBox (std::vector<CollisionTempInfo>& collisionsTempInfo, std::vector<OrientedBoxCollider *>& pOrientedBoxColliders)
{
    for (std::vector<CollisionTempInfo>::iterator it = collisionsTempInfo.begin(); it != collisionsTempInfo.end(); it++)
    {
        bool collisionHappend = false;
        Vec3 AB = (*it).currentVelocity * TimeSystem::getFixedDeltaTime();
        Intersection intersection;
        Sphere globalSphere = (*it).collider->getGlobalSphere();
        globalSphere.setCenter((*it).position);
        GameObject *currentGameObjectHit = nullptr;
        float firstIntersectionDistance = std::numeric_limits<float>().max();

        /*Try all collision and sort to obtain the first collision*/
        for (OrientedBoxCollider *pOrientedBoxCollider : pOrientedBoxColliders)
        {
            OrientedBox globalOrientedBox = pOrientedBoxCollider->getGlobalOrientedBox();

            /*Try firstly with sphere bounding box*/
            float maxHalfLenght = std::max(globalOrientedBox.getExtK(), std::max(globalOrientedBox.getExtI(), globalOrientedBox.getExtJ()));
            if ((globalOrientedBox.getReferential().origin - globalSphere.getCenter()).length() > (globalSphere.getRadius() + maxHalfLenght) * 2.f)
            {
                continue;
            }

            Intersection newIntersection;
            if (MovingSphereOrientedBox::isMovingSphereOrientedBoxCollided(globalSphere, globalOrientedBox, AB, newIntersection))
            {
                /*If the point is inside the collider, back oof the point*/
                if (newIntersection.intersectionType == EIntersectionType::InfinyIntersection)
                {
                    globalSphere.setCenter(globalSphere.getCenter() - AB);
                    if (!MovingSphereOrientedBox::isMovingSphereOrientedBoxCollided(globalSphere, globalOrientedBox, AB, newIntersection))
                    {
                        continue;
                    }
                    currentGameObjectHit = &pOrientedBoxCollider->getGameObject();
                    firstIntersectionDistance = 0.f;
                    intersection = newIntersection;
                    collisionHappend = true;
                    break;
                }

                /*Check if false intersection*/
                if (!(Vec3::dot(AB, newIntersection.intersection1 - globalSphere.getCenter()) > 0.f && (newIntersection.intersection1 - globalSphere.getCenter()).length() < AB.length()))
                {
                    continue;
                }

                /*Else if intersection is good, check if its the first object meet*/
                keepNearestGameObject(currentGameObjectHit, intersection, firstIntersectionDistance, newIntersection, &pOrientedBoxCollider->getGameObject(), globalSphere.getCenter());
                collisionHappend = true;
            }     
        }

        if (collisionHappend)
        {
            /*Compute tAP and tPB*/
            Vec3 OP = intersection.intersection1; /*Position of the sphere at the collision*/
            Vec3 OA = globalSphere.getCenter();
            Vec3 OB = OA + AB;
            float ABLength = AB.length();
            float tPB = ABLength > std::numeric_limits<float>::epsilon() ? (OB - OP).length() / ABLength : 0.f;

            // /*Compute the new position and the new velocity of the entity*/
            Vec3 velocity = (*it).currentVelocity;
            Vec3 reactionForce = intersection.normalI1 * -velocity.dotProduct(intersection.normalI1);
            Vec3 newVelocity = (velocity + reactionForce) * (*it).collider->getFriction() + reactionForce * (*it).collider->getBounciness();
            Vec3 remainingVelocity = newVelocity * tPB;
            Vec3 newPos = intersection.intersection1 + intersection.normalI1 * 0.001f;
            Vec3 angularVelocity =  -Vec3::cross(((intersection.intersection1 - intersection.normalI1) - OA), remainingVelocity);

            (*it).position = newPos;
            (*it).newVelocity = newVelocity;
            (*it).newAngularVelocity = angularVelocity;
            (*it).currentVelocity = remainingVelocity;

            /*Assign both game object collinding on the hit indo and call OnCollisionEnter function*/
            HitInfo hitInfo1{intersection, currentGameObjectHit, 0.f /*Static object*/};
            HitInfo hitInfo2{intersection, &(*it).collider->getGameObject(), velocity.length()};
            (*it).collider->OnCollisionEnter(hitInfo1);
            currentGameObjectHit->getComponent<Collider>()->OnCollisionEnter(hitInfo2);
        }
        else
        {
            (*it).collider->getGameObject().setTranslation((*it).position + (*it).currentVelocity * TimeSystem::getFixedDeltaTime());
            (*it).physicalObject->setAngularVelocity((*it).newAngularVelocity);
            (*it).physicalObject->getGameObject().rotate((*it).physicalObject->getAngularVelocity() * TimeSystem::getFixedDeltaTime());
            (*it).physicalObject->setVelocity((*it).newVelocity);
            (*it).physicalObject->setDirtyFlag(false);

            it = collisionsTempInfo.erase(it);

            if (collisionsTempInfo.empty() || (it == collisionsTempInfo.end()))
                return;
        }
    }
}

void PhysicSystem::update() noexcept
{
    for (PhysicalObject *object : pPhysicalObjects)
    {
        if (!object || object->isKinematic() || object->isSleeping() || !object->isActivated())
            continue;

        if (object->useGravity())
            object->addForce(gravity * object->getMass() * TimeSystem::getFixedDeltaTime());
    }

    std::vector<CollisionTempInfo> collisionsTempInfo;
    collisionsTempInfo.reserve(pColliders.size());

    std::vector<OrientedBoxCollider *> pOrientedBoxColliders;
    pOrientedBoxColliders.reserve(pColliders.size());

    for (auto &&i : pColliders)
    {
        /*Only add moving object*/
        SphereCollider* pSphereCollider =  dynamic_cast<SphereCollider *>(i);

        if (pSphereCollider)
        {
            if (i->GetAttachedPhysicalObject() && !i->GetAttachedPhysicalObject()->isKinematic())
            {
                collisionsTempInfo.push_back(CollisionTempInfo{pSphereCollider,
                                                i->GetAttachedPhysicalObject(),
                                                pSphereCollider->getGlobalSphere().getCenter(),
                                                i->GetAttachedPhysicalObject()->getVelocity(),
                                                i->GetAttachedPhysicalObject()->getAngularVelocity(), 
                                                i->GetAttachedPhysicalObject()->getVelocity()});
            }
            continue;
        }

        OrientedBoxCollider* pOrienterdBocxCollider =  dynamic_cast<OrientedBoxCollider *>(i);

        if (pOrienterdBocxCollider)
        {
            pOrientedBoxColliders.push_back(pOrienterdBocxCollider);
        }
    }

    unsigned int maxIt = 10;
    while (!collisionsTempInfo.empty())
    {
        checkCollisionBetweenMovingSphereAndStaticBox (collisionsTempInfo, pOrientedBoxColliders);
        maxIt--;
        if (maxIt == 0)
            break;
    }
    if (maxIt == 0)
    {
        SLog::logWarning("Risk of infinit collision (iteration > 10)");
    }

    for (PhysicalObject *object : pPhysicalObjects)
    {
        if (!object || object->isKinematic() || object->isSleeping() || !object->isDirty() || !object->isActivated())
            continue;

        /*update movement and torque induct by the differente force on the object*/
        object->getGameObject().translate(object->getVelocity() * TimeSystem::getFixedDeltaTime());
        object->getGameObject().rotate(object->getAngularVelocity() * TimeSystem::getFixedDeltaTime());
    }
}

inline bool ifCollisionAffectPhysicalObject(bool collision, PhysicalObject *physicalObject, HitInfo &rayHitInfo)
{
    if (collision)
    {
        rayHitInfo.gameObject = &physicalObject->getGameObject();
        return true;
    }
    return false;
}

bool PhysicSystem::rayCast(const Engine::Core::Maths::Shape3D::Segment &ray, HitInfo &rayHitInfo) noexcept
{
    bool collisionHappend = false;
    GameObject *currentGameObjectHit = nullptr;
    float firstIntersectionDistance = std::numeric_limits<float>().max();
    Intersection newIntersection;

    for (Collider *collider : pColliders)
    {
        AABBCollider *aabbColliderPtr = dynamic_cast<AABBCollider *>(collider);
        if (aabbColliderPtr != nullptr)
        {
            if (SegmentAABB::isSegmentAABBCollided(ray, aabbColliderPtr->getGlobalAABB(), newIntersection))
            {
                keepNearestGameObject(currentGameObjectHit, rayHitInfo.intersectionsInfo, firstIntersectionDistance, newIntersection, &aabbColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        SphereCollider *sphereColliderPtr = dynamic_cast<SphereCollider *>(collider);
        if (sphereColliderPtr != nullptr)
        {
            if (SegmentSphere::isSegmentSphereCollided(ray, sphereColliderPtr->getGlobalSphere(), newIntersection))
            {
                keepNearestGameObject(currentGameObjectHit, rayHitInfo.intersectionsInfo, firstIntersectionDistance, newIntersection, &sphereColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        OrientedBoxCollider *orientedBoxColliderPtr = dynamic_cast<OrientedBoxCollider *>(collider);
        if (orientedBoxColliderPtr != nullptr)
        {
            if (SegmentOrientedBox::isSegmentOrientedBoxCollided(ray, orientedBoxColliderPtr->getGlobalOrientedBox(), newIntersection))
            {
                keepNearestGameObject(currentGameObjectHit, rayHitInfo.intersectionsInfo, firstIntersectionDistance, newIntersection, &orientedBoxColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        PlaneCollider *planeColliderPtr = dynamic_cast<PlaneCollider *>(collider);
        if (planeColliderPtr != nullptr)
        {
            if (SegmentPlane::isSegmentPlaneCollided(ray, planeColliderPtr->getGlobalPlane(), newIntersection))
            {
                keepNearestGameObject(currentGameObjectHit, rayHitInfo.intersectionsInfo, firstIntersectionDistance, newIntersection, &planeColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        QuadCollider *quadColliderPtr = dynamic_cast<QuadCollider *>(collider);
        if (quadColliderPtr != nullptr)
        {
            if (SegmentQuad::isSegmentQuadCollided(ray, quadColliderPtr->getGlobalQuad(), newIntersection))
            {
                keepNearestGameObject(currentGameObjectHit, rayHitInfo.intersectionsInfo, firstIntersectionDistance, newIntersection, &quadColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        CapsuleCollider *capsuleColliderPtr = dynamic_cast<CapsuleCollider *>(collider);
        if (capsuleColliderPtr != nullptr)
        {
            if (SegmentCapsule::isSegmentCapsuleCollided(ray, capsuleColliderPtr->getGlobalCapsule(), newIntersection))
            {
                keepNearestGameObject(currentGameObjectHit, rayHitInfo.intersectionsInfo, firstIntersectionDistance, newIntersection, &capsuleColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        InfiniteCylinderCollider *infiniteCylinderColliderPtr = dynamic_cast<InfiniteCylinderCollider *>(collider);
        if (infiniteCylinderColliderPtr != nullptr)
        {
            if (SegmentInfiniteCylinder::isSegmentInfiniteCylinderCollided(ray, infiniteCylinderColliderPtr->getGlobalInfiniteCylinder(), newIntersection))
            {
                keepNearestGameObject(currentGameObjectHit, rayHitInfo.intersectionsInfo, firstIntersectionDistance, newIntersection, &infiniteCylinderColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }

        CylinderCollider *cylinderColliderPtr = dynamic_cast<CylinderCollider *>(collider);
        if (cylinderColliderPtr != nullptr)
        {
            if (SegmentCylinder::isSegmentCylinderCollided(ray, cylinderColliderPtr->getGlobalCylinder(), newIntersection))
            {
                keepNearestGameObject(currentGameObjectHit, rayHitInfo.intersectionsInfo, firstIntersectionDistance, newIntersection, &cylinderColliderPtr->getGameObject(), ray.getPt1());
                collisionHappend = true;
            }
            continue;
        }
    }

    rayHitInfo.gameObject = currentGameObjectHit;
    return collisionHappend;
}

bool PhysicSystem::rayCast(const Vec3 &origin, const Vec3 &direction, float maxDistance, HitInfo &rayHitInfo) noexcept
{
    return rayCast(Segment{origin, origin + maxDistance * direction}, rayHitInfo);
}

bool PhysicSystem::rayCast(const Vec3 &pt1, const Vec3 &pt2, HitInfo &rayHitInfo) noexcept
{
    return rayCast(Segment{pt1, pt2}, rayHitInfo);
}

bool PhysicSystem::triggerRayCast(Engine::Ressources::GameObject *pTriggerGameObject, const Engine::Core::Maths::Shape3D::Segment &ray, Engine::Physics::ColliderShape::HitInfo &rayHitInfo) noexcept
{
    if (rayCast(ray, rayHitInfo))
    {
        Collider* pCollider = rayHitInfo.gameObject->getComponent<Collider>();
        HitInfo hitInfo {rayHitInfo.intersectionsInfo, pTriggerGameObject, ray.getLenght()};
        pCollider->OnCollisionEnter(hitInfo);    
        return true;
    }
    return false;
}

bool PhysicSystem::triggerRayCast(Engine::Ressources::GameObject *pTriggerGameObject, const Engine::Core::Maths::Vec3 &origin, const Engine::Core::Maths::Vec3 &direction, float maxDistance, Engine::Physics::ColliderShape::HitInfo &rayHitInfo) noexcept
{
    return triggerRayCast(pTriggerGameObject, Segment{origin, origin + maxDistance * direction}, rayHitInfo);
}

bool PhysicSystem::triggerRayCast(Engine::Ressources::GameObject *pTriggerGameObject, const Engine::Core::Maths::Vec3 &pt1, const Engine::Core::Maths::Vec3 &pt2, Engine::Physics::ColliderShape::HitInfo &rayHitInfo) noexcept
{
    return triggerRayCast(pTriggerGameObject, Segment{pt1, pt2}, rayHitInfo);
}

bool PhysicSystem::triggerRayCast(const std::string &tag, const Engine::Core::Maths::Shape3D::Segment &ray, Engine::Physics::ColliderShape::HitInfo &rayHitInfo) noexcept
{
    if (rayCast(ray, rayHitInfo))
    {
        Collider *pCollider = rayHitInfo.gameObject->getComponent<Collider>();
        GameObject tempGOWithTag;
        tempGOWithTag.setTag(tag);
        HitInfo hitInfo1 {rayHitInfo.intersectionsInfo, &tempGOWithTag, ray.getLenght()};
        pCollider->OnCollisionEnter(hitInfo1);    
        return true;
    }
    return false;
}

bool PhysicSystem::triggerRayCast(const std::string &tag, const Engine::Core::Maths::Vec3 &origin, const Engine::Core::Maths::Vec3 &direction, float maxDistance, Engine::Physics::ColliderShape::HitInfo &rayHitInfo) noexcept
{
    return triggerRayCast(tag, Segment{origin, origin + maxDistance * direction}, rayHitInfo);
}

bool PhysicSystem::triggerRayCast(const std::string &tag, const Engine::Core::Maths::Vec3 &pt1, const Engine::Core::Maths::Vec3 &pt2, Engine::Physics::ColliderShape::HitInfo &rayHitInfo) noexcept
{
    return triggerRayCast(tag, Segment{pt1, pt2}, rayHitInfo);
}