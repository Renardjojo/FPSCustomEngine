#include "GE/Core/Maths/Collision.hpp"
#include "GE/Core/Maths/vec.hpp"

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::Shape3D::Linked;

bool Collision::IsSphereOrientedBoxCollided(Sphere sphere, OrientedBox box, CollisionPoints& _intersection)
{
    /*Step 1 : Find if the both shape are non collided*/
    //looking for a collision between the bow and the AABB of the sphere.
    Vec3 BOmega = Referential::GlobalToLocalPosition(box.getReferential(), sphere.getCenter());

    if (std::abs(BOmega.x) > box.getExtI() + sphere.getRadius() ||
        std::abs(BOmega.y) > box.getExtJ() + sphere.getRadius() ||
        std::abs(BOmega.z) > box.getExtK() + sphere.getRadius())
    {
        _intersection.SetNotIntersection();
        return false;
    }

    /*Step 2 : find the near point from box to the sphere*/
    Vec3 pointInBoxNearestOfSphere = Vec3({ std::clamp(BOmega.x, -box.getExtI(), box.getExtI()),
                                            std::clamp(BOmega.y, -box.getExtJ(), box.getExtJ()),
                                            std::clamp(BOmega.z, -box.getExtK(), box.getExtK())});

    /*Step 3 : looking for a collision point*/
    if (std::abs(pointInBoxNearestOfSphere.length() - BOmega.length()) <= sphere.getRadius())
    {
        _intersection.SetOneIntersection(Referential::LocalToGlobalPosition(box.getReferential(), pointInBoxNearestOfSphere));
        return true;
    }

    _intersection.SetNotIntersection();
    return false;
}

// linked
bool Collision::IsSphereOrientedBoxCollided(LinkedSphere sphere, LinkedOrientedBox box, CollisionPoints& _intersection)
{
    /*Step 1 : Find if the both shape are non collided*/
    //looking for a collision between the bow and the AABB of the sphere.
    Vec3 BOmega = Referential::GlobalToLocalPosition(box.getReferential(), sphere.getCenter());

    if (std::abs(BOmega.x) > box.getExtI() + sphere.getRadius() ||
        std::abs(BOmega.y) > box.getExtJ() + sphere.getRadius() ||
        std::abs(BOmega.z) > box.getExtK() + sphere.getRadius())
    {
        _intersection.SetNotIntersection();
        return false;
    }

    /*Step 2 : find the near point from box to the sphere*/
    Vec3 pointInBoxNearestOfSphere = Vec3({ std::clamp(BOmega.x, -box.getExtI(), box.getExtI()),
                                            std::clamp(BOmega.y, -box.getExtJ(), box.getExtJ()),
                                            std::clamp(BOmega.z, -box.getExtK(), box.getExtK())});

    /*Step 3 : looking for a collision point*/
    if (std::abs(pointInBoxNearestOfSphere.length() - BOmega.length()) <= sphere.getRadius())
    {
        _intersection.SetOneIntersection(Referential::LocalToGlobalPosition(box.getReferential(), pointInBoxNearestOfSphere));
        return true;
    }

    _intersection.SetNotIntersection();
    return false;
}

bool Collision::GetFirstCollisionPointSphereBox(LinkedOrientedBox box, LinkedSphere sphere, Vec3 vectSphereSpeed, float deltaTime, HitInfo& hit)
{
    CollisionPoints collisionPoint;

    box.setLocalExtI(box.getLocalExtI() + sphere.getRadius());
    box.setLocalExtJ(box.getLocalExtJ() + sphere.getRadius());
    box.setLocalExtK(box.getLocalExtK() + sphere.getRadius());

    // if (!Collision::IsSegmentIntersectingOrientedBox(sphere, box, collisionPoint))
    //     return false;

    // detect if intersection point is on voroin face
    // is recherched point

    // if not detect if intersecting 1 or 3 near captulse

    // return info
    //collisionPoint.
    return false;
}
